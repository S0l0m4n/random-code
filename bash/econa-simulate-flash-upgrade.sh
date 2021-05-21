#!/bin/bash
# Takes a Hub or SDU image and pads it out to fit in the available space from
# the app start address (0x802.0000). Additionally, the following boot data is
# written:
#
#     addr    |        data         |   meaning
#  -----------------------------------------------
#   end - 8   | ffff.ff00.aaaa.aaaa | curr app = A
#   end - 16  | ffff.fff0.bbbb.bbbb | next app = B, to be installed
#   end - 24  | 900d.900d.900d.900d | self-check = good (A)
#
# NOTE: Actually, the field values have the opposite endianness, so the values
# actually are:
#
#     addr    |        data         |   meaning
#  -----------------------------------------------
#   end - 8   | aaaa.aaaa.00ff.ffff | curr app = A
#   end - 16  | bbbb.bbbb.f0ff.ffff | next app = B, to be installed
#   end - 24  | 0d90.0d90.0d90.0d90 | self-check = good (A)
#
# This is something I overlooked in designing the bootloader. Anyway, the values
# are specified correctly for the echo command below.
#
# ---
#
# With this boot data, the bootloader will think that the app is running for the
# first time and take any one-time action that occurs in this state...
#
# Well, that was the intention. In fact, the bootloader will try to copy the
# new app image from EXT MEM, overwriting the image in INT MEM that this script
# prepares. So actually, this script is useful - DOH! Anyway, I keep it for
# reference on how to use dd, as it does generate the image described above,
# with the specified boot data.
#
# ---
#
#   end = 0x80000 for SDU (only this is implemented)
#       = 0xc0000 for Hub

END_VALUE=0x80000

MAX_SIZE=$((END_VALUE - 0x20000))

function usage
{
    echo "Usage: $0 -in <in-img> -out <out-img>"
    echo "  -in   : input image (after building)"
    echo "  -out  : output image (with padding and boot data)"
    exit
}

while [ $# -gt 1 ]
do
    case $1 in
        "-in")
            shift
            in_img=$1
            ;;

        "-out")
            shift
            out_img=$1
            ;;

        *)
            usage
            ;;
    esac
    shift
done

if [[ ! -e $in_img || -z $out_img ]]
then
    usage
    exit 1
fi

# compute current image size
img_size=`ls -l $in_img | cut -d' ' -f5`

# work out how much is needed
padding_size=$((MAX_SIZE - img_size))

# first copy input to output
cp $in_img $out_img

# seek to end of file and pad with all zeros until we have a full-size image
dd if=/dev/zero of=$out_img bs=1 count=$((padding_size)) seek=$img_size

out_size=`ls -l $out_img | cut -d' ' -f5`

# write the above boot data
echo -ne "\x0d\x90\x0d\x90\x0d\x90\x0d\x90" | \
  dd of=$out_img obs=1 seek=$((MAX_SIZE - 24))
echo -ne "\xbb\xbb\xbb\xbb\xf0\xff\xff\xff" | \
  dd of=$out_img obs=1 seek=$((MAX_SIZE - 16))
echo -ne "\xaa\xaa\xaa\xaa\x00\xff\xff\xff" | \
  dd of=$out_img obs=1 seek=$((MAX_SIZE - 8))
