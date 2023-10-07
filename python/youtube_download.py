# Download the audio files for all of the video URLs given in the input file

import argparse
import os
import sys

def download_song(artist, song, url):
    file_name = '"' + artist + "__" + song + ".mp3" + '"'
    os.system(f"touch {file_name}")
    return True

def mark_completed(f, line):
    """
    Mark the song on the current line in the given file as downloaded. Change
    the mark from `-` to `#`.

    Args:
        f (file)        : A handle to the songs file, open at the current song
        line (string)   : The currently read line (containing the song)

    Returns:
        Nothing

    NOTE: The file must be open in binary format.
    """
    try:
        marker_position = line.decode().index("| -") + 2  # add 2 to reach `-`
        f.seek(-len(line), os.SEEK_CUR)  # move back to the start of the current line
        f.seek(marker_position, os.SEEK_CUR)  # move to the position of the marker
        f.write(b"#")  # write the mark completed symbol (`-` -> `#`)
        f.readline()  # move to the next line (read past the current `\n`)
    except ValueError:
        # Could not find `-` marker
        pass
    except Exception as e:
        # Here's a generic way for printing where the exception occurred
        _, _, e_traceback = sys.exc_info()
        print(f"Error:{e_traceback.tb_lineno}: {e}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Download music from YouTube.")
    parser.add_argument("-songs_file", type=str, help="List of songs to download")
    args = parser.parse_args()

    if args.songs_file is None:
        print("Please supply songs file")
        exit(0)

    with open(args.songs_file, 'rb+') as f:
        for line in f:
            # Extract the song name and look up video
            fields = line.decode().split('|')

            song = fields[0]
            artist = fields[1]
            try:
                url = fields[2]
            except IndexError:
                # Cannot download this song
                continue

            if (download_song(song, artist, url)):
                mark_completed(f, line)
