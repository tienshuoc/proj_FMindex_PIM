#include<iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdint.h>
#include <math.h>
#include <fstream>

#define STEP 2
#define OCC_SAMPLING_DIST 1

// using namespace std;

// void print_vector(std::vector<auto> my_vec)
// {
//     for(auto &el : my_vec)
//     {
//         std::cout << my_vec << " ";
//     }
//     std::cout << std::endl;
// }

std::vector<std::string> rotate_and_sort(const std::string &reference_string)
{
    std::vector<std::string> rotated_and_sorted_strings;

    std::string current_rotation = reference_string;
    // std::cout << current_rotation.size() << std::endl;
    for(size_t i = 0; i < reference_string.length(); ++i)
    {
        current_rotation.push_back(current_rotation[0]);
        current_rotation.erase(current_rotation.begin());
        rotated_and_sorted_strings.push_back(current_rotation);
    }
    std::sort(rotated_and_sorted_strings.begin(), rotated_and_sorted_strings.end());
    return rotated_and_sorted_strings;
}

uint32_t encode_to_int(std::string string_to_encode)
{
    uint32_t encoded_value = 0;
    for(size_t i = 0; i < string_to_encode.size(); ++i)
    {
        switch(string_to_encode[i])
        {
            case '$':
                // encoded_value += 0 * pow(5, i);
            break;
            case 'A':
                encoded_value += 1 * pow(5, i);
            break;
            case 'C':
                encoded_value += 2 * pow(5, i);
            break;
            case 'G':
                encoded_value += 3 * pow(5, i);
            break;
            case 'T':
                encoded_value += 4 * pow(5, i);
            break;
        }
    }
    return encoded_value;
}

std::string decode_to_string(uint32_t int_to_decode)
{
    std::string decoded_string;
    for(int i = STEP - 1; i >= 0; --i)
    {
        switch((int_to_decode / (int)pow(5, i)))
        {
            case 0:
                decoded_string.insert(0, "$");
            break;

            case 1:
                decoded_string.insert(0, "A");
            break;

            case 2:
                decoded_string.insert(0, "C");
            break;

            case 3:
                decoded_string.insert(0, "G");
            break;

            case 4:
                decoded_string.insert(0, "T");
            break;

            default:
                std::cout << "UNRECOGNIZED CHARACTER DURING DECODE!!\n";
        }
        int_to_decode %= (int)pow(5, i);
    }
    return decoded_string;
}

void extract_tables(const std::vector<std::string> &rotated_and_sorted_strings, std::vector<size_t> &F_offsets, std::vector<uint32_t> &L_column, std::vector<std::vector<uint32_t>> &occ_table)
{
    std::vector<uint32_t> running_occ(pow(5, STEP), 0);
    // std::cout << running_occ.size() << std::endl;
    for(size_t i = 0; i < rotated_and_sorted_strings.size(); ++i)
    {
        // F_offsets
        std::string F_string = rotated_and_sorted_strings[i].substr(0, STEP);
        uint32_t encoded_F = encode_to_int(F_string);
        if(F_offsets[encoded_F] == 0)
        {
            F_offsets[encoded_F] = i + 1;
        }

        uint32_t encoded_L = encode_to_int(rotated_and_sorted_strings[i].substr(rotated_and_sorted_strings.size() - STEP));
        // L_column
        L_column.push_back(encoded_L);

        // occ_table
        ++running_occ[encoded_L];
        if(i % OCC_SAMPLING_DIST == 0)
        {
            // std::cout << "sampled at " <<  i << std::endl;
            occ_table.push_back(running_occ);
        }
    }

}


int main()
{
    const std::string reference_string = "ATCGCGCGCGCATCG$";
    // std::cout << reference_string.length() << std::endl;

    std::vector<std::string> sorted_strings = rotate_and_sort(reference_string);
    std::vector<size_t> F_offsets(pow(5, STEP), 0);
    std::vector<uint32_t> L_column;
    std::vector<std::vector<uint32_t>> occ_table;

    // print rotated strings
    for(std::string& _str : sorted_strings)
    {
        std::cout << _str << std::endl;
    }
   
    extract_tables(sorted_strings, F_offsets, L_column, occ_table);

    // print F offsets
    std::cout << "printing F offsets:" << std::endl;
    for(size_t i = 0; i < F_offsets.size(); ++i)
    {
        std::cout << decode_to_string(i) << ": ";
        std::cout << F_offsets[i] << std::endl;
    }

    // print L column
    std::cout << "printing L column:" << std::endl;
    for(uint32_t a : L_column)
    {
        std::cout << decode_to_string(a) << std::endl;
    }

    // print occ table
    std::cout << "printing Occ table:" << std::endl;
    for(size_t i = 0; i < occ_table[0].size(); ++i)
    {
        std::cout << decode_to_string(i) << " ";
    }
    std::cout << std::endl;
    for(auto &occ_entry : occ_table)
    {
        for(uint32_t &occ : occ_entry)
        {
            std::cout << occ << std::string(STEP, ' ');
        }
        std::cout << std::endl;
    }



    // write to files
    std::fstream table;
    table.open("table.txt", std::ios::out);
    for(int a : F_offsets)
    {
        table << a << " ";
    }
    for(uint32_t a : L_column)
    {
        table << a << " ";
    }
    for(auto &occ_entry : occ_table)
    {
        for(uint32_t &occ : occ_entry)
        {
            table << occ << " ";
        }
    }
    table << "\n";
}