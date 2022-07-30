#!/usr/bin/bash

# Examples
# source converter.sh /home/pi/Pictures/test /tmp/output
# source converter.sh /home/pi/Pictures/test /tmp/output 640x480


# The first argument
SOURCE="$1"
# The second argument
DESTINATION="$2"
# The third argument 
DIMENSION="$3"


# Terminate the script if the $SOURCE or $DESTINATION variable is not defined
if [ -z "$SOURCE" ] || [ -z "$DESTINATION" ]; then
	echo "FF"
  return 1 2>/dev/null
  exit 1
fi


# The condition below checks if the convert utility is already installed
# Otherwise it installs the Imagemagick package
if [ -z $(which convert | head -n 1) ]; then
  sudo apt-get install imagemagick
fi


# The following command creates a $DESTINATION directory in case it doesn't exist
echo abc
mkdir -p $DESTINATION


for file in "$SOURCE"/*.{jpg,jpeg,png,gif}; do 
	echo "F: $file"
  if [ -f "$file" ]; then 
    # getting the file name
    filename=$(basename "$file")
    #removing all characters that are not: a-z,0-9,-,_,.,?,`,`
    filename=$(echo "$filename" | sed -r 's/[^a-z0-9A-Z_\-\.\?\,]+//g')
    #~ echo "$file || $filename"

    filename=$(echo "$filename" | sed -r 's/.jpg|.gif/.png/g')
    echo "$filename"
    
    if [ -z "$DIMENSION" ]; then
      # all images will have the initial size
      convert "$file" "$DESTINATION/$filename"
    else
      # All images will be resized
      convert -resize "$DIMENSION" "$file" "$DESTINATION/$filename"
    fi
  fi 
done

