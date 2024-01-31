# Download the audio files for all of the video URLs given in the input file

import argparse
import os
import sys

youtube_cmd = \
        "youtube-dl --extract-audio --audio-format mp3 -o \"{file_name}\" {url}"

def download_song(artist, song, url):
    artist_part = '-'.join(artist.lower().split())
    song_part = '-'.join(song.lower().split())
    file_name = artist_part + "__" + song_part + ".%(ext)s"
    # NOTE: We are already in the songs dir
    # NOTE: The `os.system` command returns 0 on success
    return (artist_part + "__" + song_part + ".mp3" in os.listdir('.')
        or  0 == os.system(youtube_cmd.format(file_name=file_name, url=url)))

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
        f.flush()
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
    parser.add_argument("-songs_file", type=str, help="list of songs to download")
    parser.add_argument("-out_dir", type=str, help="download directory")
    args = parser.parse_args()

    if args.songs_file is None:
        print("Please supply songs file")
        exit(0)
    elif args.out_dir is None:
        print("Please supply download directory")
        exit(0)

    try:
        os.listdir(args.out_dir)
    except FileNotFoundError:
        # Download directory does not exist, so create it
        try:
            os.mkdir(args.out_dir)
        except:
            print("Could not create download directory")
            exit(0)

    home_dir = os.getcwd()

    with open(args.songs_file, 'rb+') as f:
        # Move to download directory for placing songs
        os.chdir(args.out_dir)

        for line in f:
            # Extract the song name and look up video, cleaning up white space
            fields = [ field.strip() for field in  line.decode().split('|') ]

            song = fields[0]
            artist = fields[1]
            try:
                url = fields[2]
                try:
                    mark  = fields[3]
                    if mark == '#':
                        print(f"Already downloaded: {song}")
                        continue
                except IndexError:
                    # Song not marked with `-` or `#`
                    print(f"Mark not found: {song}")
                    continue

            except IndexError:
                # Cannot download this song
                continue

            if download_song(artist, song, url):
                mark_completed(f, line)

    if (home_dir != os.getcwd()):
        # Return to original directory
        os.chdir(home_dir)
