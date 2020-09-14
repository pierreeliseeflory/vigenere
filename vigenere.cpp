#include <fstream>
#include <cstring>
#include <iostream>
#include <cmath>

std::string ReadFile(std::string file) {
    std::string text;

    std::ifstream crypted_file(file);

    if (crypted_file.is_open()) {
        getline(crypted_file, text);
    }
    crypted_file.close();
    return text;
}

int KeySizeVigenere (int* values, int key_max_size, int string_length) {
    float ic, ic_avg;
    int occurences[26];
    int length_substring;
    for (int m = 3; m < key_max_size; ++m) {
        ic_avg = 0;

        for (int i = 0; i < m; ++i) {
            ic = 0;
            length_substring = 0;

            for (int letter = 0; letter <= 25; ++letter) {
                occurences[letter] = 0;
            }
            for (int temp = 0; temp*m + i < string_length; ++temp) {
                ++occurences[values[temp*m + i]];
                ++length_substring;
            }

            for (int letter = 0; letter <= 25; ++letter) {
                ic += occurences[letter]*(occurences[letter]-1);
            }
            ic = ic / (length_substring*(length_substring-1));
            ic_avg += ic;
        }

        ic_avg /= m;
        if (std::abs(ic_avg - 0.071) < 0.01) {
            std::cout << "Taille de la clé: " << m << std::endl;
            return m;
        }
    }
    std::cout << "Aucune clé de taille inférieure à " << key_max_size << " trouvée." << std::endl;
    return -1;
}

std::string EncodeVigenere(std::string file, std::string key) {
    
    std::string clear = ReadFile(file);
    
    int length = clear.length();
    int key_length = key.length();
    std::string crypted_text = "";

    for (int i = 0; i < length; ++i) {
        crypted_text += (((clear[i] - 'A') + (key[i%key_length] - 'A') + 26)%26 + 'A');
    }
    return crypted_text;
}

void DecodeVigenere (std::string chiffre) {

    std::string crypted = ReadFile(chiffre);

    int string_length = crypted.length();
    int values[string_length];

    for (int temp = 0; temp < string_length; ++temp) {
        values[temp] = crypted[temp] - 'A';
    }

    int key_length = -1;
    key_length = KeySizeVigenere(values, 16, string_length);

    if (key_length == -1) {
        std::cout << "Impossible de déchiffrer ce message." << std::endl;
        return;
    }


    int occurences[key_length][26] = {0};
    int length_substring[key_length] = {0};


    for (int i = 0; i < key_length; ++i) {
        for (int temp = 0; temp*key_length + i < string_length; ++temp) {
            ++occurences[i][values[temp*key_length + i]];
            ++length_substring[i];
        }
    }

    float max_icm, icm;
    int best_offset;
    int offset[key_length-1];

    for (int i = 1; i < key_length; ++i) {
        best_offset = 0;
        max_icm = 0;
        for (int letter = 1; letter < 26; ++letter) {
            max_icm += occurences[0][letter]*occurences[i][letter];
        }
        max_icm = max_icm / (length_substring[0]*length_substring[i]);
        for (int g = 1; g < 26; ++g) {
            icm = 0;
            for (int letter = 1; letter < 26; ++letter) {
                icm += occurences[0][letter]*occurences[i][(letter-g+26)%26];
            }
            icm = icm / (length_substring[0]*length_substring[i]);
            if (icm > max_icm) {
                max_icm = icm;
                best_offset = g;
            }
        }
        std::cout << "Décalage lettre n°" << i+1  << "; g= " << best_offset << std::endl;
        offset[i-1] = best_offset;

        for (int temp = 0; i + temp*key_length < string_length; temp++) {
            values[temp*key_length + i] = (values[temp*key_length + i] + best_offset)%26;
        }
    }

    int final_occurences[26] = {0};
    for (int i = 0; i < string_length; ++i) {
        ++final_occurences[values[i]];
    }

    int max = 0, max_letter = -1;
    for (int letter = 0; letter < 26; ++letter) {
        if (final_occurences[letter] > max) {
            max = final_occurences[letter];
            max_letter = letter;
        }
    }
    int initial_offset = max_letter - ('E' -'A');
    char alphabet[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

    // Prints the key
    std::string key ="";
    key += alphabet[initial_offset];
    for (int i = 1; i < key_length; ++i) {
        key += alphabet[(initial_offset - offset[i-1] + 26)%26];
    }
    std::cout << "Clé: " << key << std::endl;

    for (int i = 0; i < string_length; ++i) {
        values[i] = (values[i] - initial_offset + 26)%26;
        std::cout << alphabet[values[i]];
    }
    std::cout << std::endl;
}

int main () {
    
    // std::string clear_text = ReadFile("Clair.txt");
    std::cout << EncodeVigenere("Clair.txt", "BEAU") << std::endl;

    // DecodeVigenere("Chiffre.txt"); 

    return 0;
}