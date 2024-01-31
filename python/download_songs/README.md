README
======
### youtube_search.py
Find the YouTube URL for each song in the given input file, which is a CSV file delimited with `|` rather than commas. The file initially has this structure:
```
<song> | <name>
```

When the URL is found on each line, we add two extra columns, the URL and the mark:
```
<song> | <name> | <url> | <mark>
```

Initially the mark we write is `-`, indicating that the song needs to be downloaded. Later, when it is downloaded, the mark will get changed to `#`, meaning this song can be skipped by the download script.

If the URL is already provided for the song on a given line, that line is skipped in the processing: we don't try to find the URL in this script, nor do we try to download it in the download script.

**NOTE:** Each time the script runs, a backup of the songs file is first generated, incrementing the current backup file count. This prevents us from overwriting any work if the script produces bogus results and ruins the current songs file. If such misfortune does occur, simply find the best backup file to resume from, copy it in place of the songs file and, after making any necessary fix, rerun the script.

Usage:
```
$: python3 youtube_search.py -songs_file <songs>
```

### youtube_download.py
Usage:
```
$: python3 youtube_download.py -songs_file <songs> -out_dir <out>
```

Both arguments must be supplied. The downloaded songs go in the supplied output directory.
