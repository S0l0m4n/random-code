import argparse
import os
from googleapiclient.discovery import build

# Set up the YouTube Data API v3
api_key = "AIzaSyDKhPSuDV82MYa-OTzu-TrrU9ij5ot1YRI" # my API key
youtube = build("youtube", "v3", developerKey=api_key)

def search_youtube(query):
    try:
        search_response = youtube.search().list(
            q=query,
            type="video",
            part="id",
            maxResults=1
        ).execute()

        if "items" in search_response:
            video_id = search_response["items"][0]["id"]["videoId"]
            video_url = f"https://www.youtube.com/watch?v={video_id}"
            print(search_response)
            return video_url
        else:
            return "No results found."

    except Exception as e:
        return str(e)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Search for a music video on YouTube.")
    parser.add_argument("song", type=str, help="Name of the song to search for on YouTube")
    args = parser.parse_args()

    song_name = args.song
    video_url = search_youtube(song_name)

    if video_url:
        print(f"Found the music video for '{song_name}':")
        print(video_url)
    else:
        print(f"No results found for '{song_name}'.")
