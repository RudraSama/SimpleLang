#pragma once
#include <fstream>
#include <vector>

/**
 * @brief Class for Handling output file
 */
class File_Writer{
    private:
        std::fstream output_file;               //!<< Output file object.
        std::vector<std::string> file_vector;   //!<< Vector to store Assembly commands;
        int position = -1;                        //!<< Counter for counting characters;

    public:

        /**
         * @brief Method for opening output file. 
         * @param string, string text to write in file.
         * @return void
         */
        void open_file(std::string &output_file_url);

        /**
         * @brief Method for writing to output file. 
         * @param string, string text to write in file.
         * @return void
         */
        void append(std::string s);

        /**
         * @brief Returns current characters in file. 
         * @return int 
         */
        int get_position();

        /**
         * @brief Sets new position for vector. 
         * @return void
         */
        void set_position(int p);

         /**
         * @brief Closing output file. 
         * @return void 
         */       
        void close_file();
};
