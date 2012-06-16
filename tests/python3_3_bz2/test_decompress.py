import sys
import os
import bz2
import time
import traceback
import pprint

bz2folder = "E:\\c10-archive-r1\\BZ2_TAR\\BZ2_TAR~\\bz2\\"

files = os.listdir(bz2folder)

print(len(files))

#test1 = files[0]
#print (test1)
#print(get_string(test1))
#bz2.decompress(get_string(test1))


def print_log(i,f, fs):
    s = "--------\n" +\
    "testcase %d: %s\n"%(i,f) +\
    "file len: %d\n"%len(fs) +\
    time.strftime("local testtime: %Y-%m-%d %H:%M:%S\n")
    return s
    

def get_string(f):
    return open(bz2folder + f,"rb").read()

excepions_set = set()



with open("testlog.txt","a") as out:
    with open("exceptions.txt","a") as exp:
        oldtime = None
        infon = 2000
        for i,f in enumerate(files):
            if i%infon == 0:
                newtime = time.time()
                if None != oldtime:
                    persec = infon / float(newtime - oldtime )
                    remaining = ( len(files) - (i+1) ) / persec
                    print("%d persec %f remaining %f"%(i,persec, remaining ))
                oldtime = newtime
                    
            fs = get_string(f)
            #printing slows testing ...
            #out.write(print_log(i,f,fs))
            try:
                _ = bz2.decompress(fs)
            except Exception as e:

                exc_type, exc_value, exc_tb = sys.exc_info()
                edesc = tuple(traceback.format_exception(exc_type, exc_value, exc_tb))

                if not edesc in excepions_set:
                    
                    excepions_set.add(edesc)
                    exp.write(print_log(i,f,fs))
                    pprint.pprint(edesc,stream=exp)
                    exp.flush()

                    print(print_log(i,f,fs))
                    pprint.pprint(edesc)
            
        
    

    
