python.exe .\Web\assembler.py
python '.\Main\Helper Scripts\game_compressor.py'
python '.\Main\Helper Scripts\login_compressor.py'
py -3 '.\Main\Helper Scripts\key_file_generator.py' 0
py -3 '.\Main\Helper Scripts\fill_main_template.py' 0
python.exe ".\Main\Helper Scripts\generate_list_of_data_filenames.py"