#include <vector>
#include <iostream>
#include <thread>
#include <fstream>
#include <algorithm>
#include <mutex>
#include <iterator>
#include <thread>
#include <experimental/filesystem>
#include <future>

namespace fs = std::experimental::filesystem;

std::mutex mutex;
size_t batch_size = 400000;      
//столько uint64_t вмещается в 4mb,так как у нас два потока, то нужно распределить для каждого поровну
//Я сделал на 10000 меньше, потому что некоторые операции могут потребовать дополнительную память при обработке

 //Будем сортировать методом слияния файлы. Воспользуемся методом merge из stl 
void merge_file(const std::string f1, const std::string f2, const std::string f_out ){
    std::ifstream f1_op(f1, std::ios::binary );
    std::ifstream f2_op(f2, std::ios::binary );
    std::ofstream mergeFile(f_out, std::ios::binary );
    std::merge(std::istream_iterator<uint64_t>(f1_op),
               std::istream_iterator<uint64_t>(),
               std::istream_iterator<uint64_t>(f2_op),
               std::istream_iterator<uint64_t>(),
               std::ostream_iterator<uint64_t>(mergeFile, " "));
   return;
}

void my_sort(const char* file, const std::string& prefix) {
    std::vector<uint64_t> vec(batch_size);
    //Исользование метода из СИ, на сколько я разузнал в интернете, более эффективно для блочного считывание, чем считывание с помощью ifstream итеративно
    FILE *fp;
    if ((fp = fopen(file, "rb")) == NULL) {
        std::cout << "Cannot open file." << std::endl;
        return;
    }
    

    int last = 0;
//считываем файл по по кусочкам, чуть меньше 8 mb, сортируем их и сразу же записываем в файл для дальнейшей сортировки слиянием
    while(!feof(fp)) {
        
        std::lock_guard<std::mutex> lock(mutex);
        fread(vec.data(), sizeof(uint64_t), batch_size, fp);
        

        std::sort(begin(vec), end(vec));
        std::ofstream out("data/" + prefix + std::to_string(last) + ".bin",
                        std::ios::binary);
        for (auto j : vec) { //записываем данные в файл, пока не дойдем до последнего символа
            if (j != 0)
                out << j << " ";
            else
                continue;
        }
        last++;
    }
    fclose(fp);


 size_t start = 0;

    std::string file_1 = "data/" + prefix + std::to_string(start) + ".bin";
    std::string file_2;
    std::string file_3;
    while (start < last-1){
        

        file_2 = "data/" + prefix + std::to_string(start + 1) + ".bin";
        file_3 = "data/new" + prefix + std::to_string(start + 1) + ".bin";
    
        merge_file(file_1, file_2, file_3);

        ++start;
        
        file_1 = "data/new" + prefix + std::to_string(start) + ".bin";
        if (start > 1){
            fs::remove("data/new" + prefix + std::to_string(start - 1) + ".bin");

        }
    }
    std::rename((file_1).c_str(),
                ("data/" + prefix + "output.bin").c_str() );
    return;
}

int main(int argc, char* argv[]) {
    if (argc != 3 ) {
        std::cout << "Please, enter names of input and output files" << std::endl;
        return 0;
    }

    fs::create_directories("data");
    auto file = argv[1];
    auto outputFile = argv[2];
    std::thread t1(my_sort, file, "one_");
    std::thread t2(my_sort, file, "two_");

    t1.join();
    t2.join();
    merge_file("data/one_output.bin", "data/two_output.bin", outputFile);

    fs::remove_all("data");
    return 0;
}

