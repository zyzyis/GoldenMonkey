#! /usr/bin/python

import sys

def formatHeader(mode, nspu, res, ref):
    resxy = {0: [640, 480], 1:[1024, 768], 2:[1280, 720]}
    nora = {0: 1, 1: 32, 2: 512, 3: 1024}
    str = '<scene resx=\"%d\" \
resy=\"%d\" \
nspu=\"%d\" \
spp=\"1\" \
accel=\"simple\" \
camera=\"%s\" \
jobber=\"cell\" \
rays_per_job=\"1024\" \
max_ray_round=\"%d\" \
simd=\"%d\">\n' % (resxy[res][0], resxy[res][1], nspu, 'perspective', ref, mode)
    return str

'''
File name syntax:
[scene]_[mode]_[nspu]_[resolution]_[reflection]
'''

if __name__ == '__main__':
    if len(sys.argv) == 1:
        print sys.argv[0] + ' [raw file]'
    else:        
        # raw file
        raw_file = open(sys.argv[1], 'r')
        # substract the file name
        filename = sys.argv[1].split('.');
        # read the content
        raw_content = raw_file.readlines();
        raw_file.close()
        # format the header
        for mode in range(2):
            for nspu in range(6):
                for res in range(3):
                    for reflection in range(3):
                        header = formatHeader(mode, nspu + 1, res, reflection + 1)
                        newfilename = '%s_%d_%d_%d_%d.xml' % (filename[0], mode, nspu + 1, res, reflection + 1)
                        newfile = open(newfilename, 'w')
                        newfile.write(header)
                        newfile.writelines(raw_content)
                        newfile.close()

    
