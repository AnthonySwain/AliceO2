import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import uproot

'''This was an idea to create and plot the hashmaps. 
Unfortunately, there are way too many voxels for matplotlib to handle...'''

# Upon a re-think, this is better to make in root using a TH3 histogram and get projections into the different planes. (Also will keep the same look as hit and step graphs.)


def visualize_3d_voxels(voxel_values,Nx,Ny,Nz):
    # Get the dimensions of the 3D voxel data  - Nx,Ny,Nz
    
    # Create a 3D grid of coordinates
    x, y, z = np.meshgrid(range(Nx), range(Ny), range(Nz))

    # Flatten the coordinates and voxel data for plotting
    x, y, z = x.flatten(), y.flatten(), z.flatten()
    voxel_data = voxel_values#.flatten()

    # Create a 3D plot
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    # Set the voxels' color based on their values (True/False)
    colors = ['b' if val else 'r' for val in voxel_data]

    # Plot the voxels as cubes
    ax.scatter(x, y, z, c=colors, marker='s', s=100)

    # Set the aspect ratio to be equal to avoid distortion
    ax.set_aspect('equal')

    # Set labels and title
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    ax.set_title('3D Voxel Visualization')

    # Show the plot
    plt.show()


def read_data_from_txt(filepath):
    '''Reads list of 0's/1's from a .txt file'''
    with open(filepath, 'r') as file:
        lines = file.read().strip()

    voxel_data = [bool(digit) for digit in lines]

    return (voxel_data)




def read_root_file(filename):
    '''Reads the hashmap from a root file. Additional implementation left as a comment/'''
    # Open the ROOT file
    root_file = uproot.open(filename, asgenobj=True)

    # Access the "Keys" and "Values" objects from the file
    #keys = root_file["Keys"].array()
    values = root_file["Values"]
    voxel_data = np.array(values.array(), dtype=np.uint8)  # dtype should match the C++ type (unsigned char)
    boolean_list = [bool(val) for val in voxel_data]

    # Convert the keys and values into dictionaries
    #voxel_map = dict(zip(keys, values))

    # Access the "VoxelMeta" object from the file
    #voxel_meta = root_file["VoxelMeta"].array()

    # Extract the meta information from the "VoxelMeta" object
    #fNx, fNy, fNz, fLowerX, fLowerY, fLowerZ, fDeltaX, fDeltaY, fDeltaZ, fInvDeltaX, fInvDeltaY, fInvDeltaZ = voxel_meta.tolist()

    # Access the "Properties" object from the file
    #properties = root_file["Properties"].array()

    # Assuming "Properties" is an array of floats, you can convert it to a numpy array
    #properties = np.array(properties)

    return boolean_list 


# Nx, Ny, Nz (i.e bin numbers require manual entry.)
Nx, Ny, Nz = 200, 200, 600
filepath = "HashMap.txt"
voxel_data = read_data_from_txt(filepath)
#voxel_data = read_root_file("hashmap.root")
visualize_3d_voxels(voxel_data, Nx, Ny, Nz)

