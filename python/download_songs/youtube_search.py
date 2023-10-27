import argparse
from googleapiclient.discovery import build
import os
import shutil

# Set up the YouTube Data API v3
api_key = "AIzaSyAdMm1c7iygWlJBx-TOAjU8NBw7UbKK1Eo" # my API key
youtube = build("youtube", "v3", developerKey=api_key)

def search_youtube(query):
    try:
        search_response = youtube.search().list(
            q=query,
            type="video",
            part="id",
            maxResults=1
        ).execute()

        if search_response['items'] != []:
            video_id = search_response["items"][0]["id"]["videoId"]
            video_url = f"https://www.youtube.com/watch?v={video_id}"
            return video_url
        else:
            print("No results found")
            return ValueError

    except Exception as e:
        print(str(e))
        return e

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Search for a music video on YouTube.")
    parser.add_argument("-songs_file", type=str, help="List of songs to look up")
    args = parser.parse_args()

    if args.songs_file is None:
        print("Please supply songs file")
        exit(0)

    # We will compose the contents of the new file in this string
    out_text = ""

    # Back up the original file
    shutil.copyfile(args.songs_file, args.songs_file + ".bak")

    with open(args.songs_file, 'r') as f:
        count = 0
        for line in f:
            # First write back the line as is without the newline
            out_text += line[:-1]

            count += 1

            # Extract the song name and look up video
            fields = line.split('|')
            song_name = fields[0].strip()

            if len(fields) > 2:
                # We already have a URL for this video, so move onto the next one
                pass
            else:
                try:
                    video_url = search_youtube(song_name)
                    print("Found the music video for #{} {:40}:"
                            .format(count, f"'{song_name}'"), end=' ')
                    print(video_url)
                    # Write the video URL to the file at the end of the line
                    out_text += f" | {video_url} | -"
                except:
                    print(f"No results found for #{count} '{song_name}'")

            # Write final new line
            out_text += '\n'

    # Write the new contents back to the original file
    with open(args.songs_file, 'w') as f:
        f.write(out_text)
