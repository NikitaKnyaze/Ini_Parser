#include <iostream>
#include <fstream>
#include <map>
#include <stdexcept>
#include <string>
#include <Windows.h>

class ini_parser {
private:
   std::map<std::string, std::map<std::string, std::string>> my_map_2;
   std::string line;
public:
   ini_parser(std::string file) {
      std::ifstream file_name{ file };
      if (!file_name.is_open()) {
         throw std::runtime_error("Файл не открылся");
      }
      std::string section;
      int error_line = 0;

      while (std::getline(file_name, line))
      {
         std::string variable, value;
         size_t pos = 0, comm_pos = 0;
         error_line++;
         if ((pos = line.find('[')) != std::string::npos)
         {
            size_t end_pos = line.find(']');
            if (end_pos != std::string::npos) {
               section = line.substr(pos + 1, end_pos - pos - 1);
            }
         }
         else if (!line.empty() && line[0] != ';')
         {
            size_t variable_pos = line.find('=');
            if (variable_pos != std::string::npos)
            {
               variable = line.substr(0, variable_pos);
               comm_pos = line.find(';');
               if (comm_pos != std::string::npos) { value = line.substr(variable_pos + 1, comm_pos - variable_pos - 1); }
               else { value = line.substr(variable_pos + 1); }
            }
         }
         my_map_2[section][variable] = value;
      }
      //Проверка мэпа
      /*for (const auto& some : my_map_2) {
         std::cout << some.first;
         for (const auto& some2 : some.second) {
            std::cout << some2.first << ' ' << some2.second << std::endl;
         }
      }*/
   }

   ~ini_parser()
   {};

   template <typename T>
   T get_value(const std::string& call_section, const std::string& key) {
      auto section_it = my_map_2.find(call_section);
      if (section_it != my_map_2.end()) {
         auto key_it = section_it->second.find(key);
         if (key_it != section_it->second.end()) {
            return key_it->second;
         }
         throw std::runtime_error("Переменная не найдена");
      }
      throw std::runtime_error("Секция не найдена");
   }

   template <>
   int get_value<int>(const std::string& call_section, const std::string& key) {
      auto section_it = my_map_2.find(call_section);
      if (section_it != my_map_2.end()) {
         auto key_it = section_it->second.find(key);

         if (key_it != section_it->second.end()) {
            return std::stoi(key_it->second);
         }
         throw std::runtime_error("Переменная не найдена");
      }
      throw std::runtime_error("Секция не найдена");
   }

   template <>
   double get_value<double>(const std::string& call_section, const std::string& key) {
      auto section_it = my_map_2.find(call_section);
      if (section_it != my_map_2.end()) {
         auto key_it = section_it->second.find(key);
         if (key_it != section_it->second.end()) {
               return std::stod(key_it->second);
         }
         throw std::runtime_error("Переменная не найдена");
      }
      throw std::runtime_error("Секция не найдена");
   }

};

int main()
{
   SetConsoleCP(1251);
   SetConsoleOutputCP(1251);
   std::string filename{ "Ini_File.txt" };
   try {
      ini_parser parser(filename);
      auto value = parser.get_value<int>("Section2", "var1");
      std::cout << value;
   }
   catch (const std::exception& err) { std::cout << "Произошла ошибка: " << err.what() << std::endl; };

   return 0;
}
