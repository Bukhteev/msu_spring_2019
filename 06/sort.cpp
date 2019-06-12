#include <vector>
#include <iostream>
#include <thread>
#include <fstream>
#include <algorithm>
#include <mutex>
#include <iterator>
#include <thread>
#include <experimental/filesystem>
#include <condition_variable>

namespace fs = std::experimental::filesystem;

std::mutex mutex;
size_t batch_size = 40000;      

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

void my_sort(const char* file, const std::string& prefix, size_t num) {
    std::vector<uint64_t> vec(batch_size);
    
    FILE *fp;
    if ((fp = fopen(file, "rb")) == NULL) {
        std::cout << "Cannot open file." << std::endl;
        return;
    }

    int last = 0;
    int start = 0;
    if (num == 1 ){
        last = 1;
        start =1;
    }

//считываем файл по по кусочкам, сортируем их и сразу же записываем в файл для дальнейшей сортировки слиянием
    while(!feof(fp)) {
        
        fseek(fp,  sizeof(uint64_t) * batch_size * last, SEEK_SET);
        fread(vec.data(), sizeof(uint64_t), batch_size, fp);
        std::sort(begin(vec), end(vec));
        std::ofstream out("data/" + prefix + std::to_string(last) + ".bin",
                        std::ios::binary);
        for (auto j : vec) { 
            if (j != 0)
                out << j << " ";
            else
                continue;
        }
    last += 2;
    }
 
    fclose(fp);

    std::string file_1 = "data/" + prefix + std::to_string(start) + ".bin";
    std::string file_2;
    std::string file_3;
    while (start < last - 1){
        
        file_2 = "data/" + prefix + std::to_string(start + 2) + ".bin";
        file_3 = "data/new" + prefix + std::to_string(start + 2) + ".bin";   
        merge_file(file_1, file_2, file_3);

        start += 2;
        file_1 = "data/new" + prefix + std::to_string(start) + ".bin";

        if (start > 3){
            fs::remove("data/new" + prefix + std::to_string(start - 2) + ".bin");

        }
    }
    std::rename((file_1).c_str(), ("data/" + prefix + "output.bin").c_str() );
    return;
}

int main(int argc, char* argv[]) {
    if (argc != 3 ) {
        std::cout << "Please, enter names of input and output files" << std::endl;
        return 0;
    }

    fs::create_directories("data");
    std::thread t1(my_sort, "in2.bin", "one_", 1);
    std::thread t2(my_sort, argv[1], "two_", 2);

    t1.join();
    t2.join();
    merge_file("data/one_output.bin", "data/two_output.bin", argv[2]);

    fs::remove_all("data");
    return 0;
}

