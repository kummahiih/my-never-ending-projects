from data_to_wav import generate_wavs
from parse_willab_data import get_data

if __name__ == "__main__":
    generate_wavs(get_data(),"willab_tempnow")
