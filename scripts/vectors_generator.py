'''
 A script to generate the search files for the lsh and hypercube algorithms.
 Search files consist of N vectors each of which is d-dimension.
'''

import argparse
import random
import os
import time

PATH = 'datasets/vectors/'
SEP = '\t'
LOWER_BOUND = 0.00
UPPER_BOUND = 100.00

def make_args_parser():
    # create an ArgumentParser object
    parser = argparse.ArgumentParser(description='Vectors Input File Generator')
    # fill parser with information about program arguments
    parser.add_argument('--o', dest='output', required=True,
                        help='Define the name of the output file')
    parser.add_argument('--d', dest='dimension', type=int, required=True,
                        help='Define the dimension of the vector space')
    parser.add_argument('--n', dest='records', type=int, required=True,
                        help='Define the number of vectors')
    # return an ArgumentParser object
    return parser.parse_args()

def print_args(args):
    print('---------------------- Vectors Input File Generator ----------------------\n')
    print("Running with the following configuration")
    # get the __dict__ attribute of args using vars() function
    args_map = vars(args)
    for key in args_map:
        print('\t',key, '-->', args_map[key])
    # add one more empty line for better output
    print()

def generate_file(file_lines, dim, output_file):
    # define output directory names
    if not os.path.exists(PATH):
        os.makedirs(PATH)
    # write in file
    with open(PATH + output_file, "w") as f:
         for i in range(file_lines):
             # write item_id
             f.write(str(i))
             f.write(SEP)
             for j in range(dim):
                 f.write(str(random.uniform(LOWER_BOUND,UPPER_BOUND)))
                 f.write(SEP)
             f.write('\n')


def main():
    # Parse command line arguments
    args = make_args_parser()
    print_args(args)
    # Generate file
    generate_file(args.records, args.dimension, args.output)

if __name__ == '__main__':
    main()
