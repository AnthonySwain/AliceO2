import random 
from os import environ
from os.path import join, abspath
O2_ROOT = environ.get("O2_ROOT")

def random_choice():
    return random.choice([0,1])

def sample_voxels(trial, n_voxels, save_file_line_by_line):
    """
    create a simple text file, each line with 0 or 1
    """
    with open(save_file_line_by_line, "w") as f:
        for i in range(n_voxels):
            on_or_off = random_choice()
            f.write(str(on_or_off)) #I don't think this is a new line


def create_hash_map(macro_path, save_file_line_by_line, nx, ny, nz, save_root_hashmap_file):
    """
    extract list from save_file, construct hash map and save to save_root_hashmap_file

    you would need to run a ROOT macro somewhat like
    """
    
    CreateHashMap = f"root -l -b -q {macro_path}({save_file_line_by_line},{save_root_hashmap_file},{nx},{ny},{nz})"
    _, hashmap_file = run_command(CreateHashMap, log_file="hits.dat") #boo 
    


#sample_voxels(10,1000,"file.txt")

print(O2_ROOT)
print (abspath(O2_ROOT))

