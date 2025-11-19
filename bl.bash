#!/usr/bin/env bash
# set the brightness for the LCD screen
# give it a number between 0-100 and it will set the screen to that brightness
#
#/sys/devices/pci0000:00/0000:00:02.0/drm/card1/card1-eDP-1/intel_backlight
if [[ -f ~/.config/bl_path ]]; then
.  ~/.config/bl_path
else
: ${BRIGHTPATH:=$(find /sys/devices -iname max_brightness |grep backlight|sed 's/max_brightness//'|sed 's/\/$//'|head -n1)}
fi
usage () {
    echo 'USAGE: bl PERCENTAGE'
    echo 'where percentage is 0-100 i.e. how bright you want the screen to be'
}
#BRIGHTPATH=$(find /sys/devices -iname max_brightness |grep backlight|sed 's/max_brightness//'|sed 's/\/$//'|head -n1)
BRIGHTNESS=$(cat $BRIGHTPATH/brightness)
MAX_BRIGHTNESS=$(cat "$BRIGHTPATH/max_brightness")
MIN_BRIGHTNESS=5
if [[ $# -eq 0 ]] ; then
  BRIGHTNESS_PERCENT=$(( BRIGHTNESS * 100 / MAX_BRIGHTNESS ))
  echo "The screen is at ${BRIGHTNESS_PERCENT}% of total brightness"
  usage
  exit 0
elif [[ $# -gt 1 ]] ; then
  usage
  exit 1
fi
brite_arg=$1
#echo max $MAX_BRIGHTNESS brite $BRIGHTNESS arg $brite_arg path $BRIGHTPATH

set_brite () {
  #echo -n $BRIGHTNESS_TARGET | sudo tee "$BRIGHTPATH/brightness"
  echo -n $BRIGHTNESS_TARGET | sudo tee $BRIGHTPATH/brightness
  echo "/$MAX_BRIGHTNESS"
  #echo -n $BRIGHTNESS_TARGET > "$BRIGHTPATH/brightness"
  #echo "echo -n $BRIGHTNESS_TARGET | sudo tee '$BRIGHTPATH/brightness' "
  # tell them what we are setting 
}

if [[ $brite_arg =~ ^\+.* ]]; then
#  echo if
  arg_stripped=$(echo $brite_arg | sed 's/^+//')
  BRIGHTNESS_TARGET=$(( BRIGHTNESS + arg_stripped ))
elif [[ $brite_arg =~ ^-.* ]]; then
#  echo elif
  arg_stripped=$(echo $brite_arg | sed 's/^-//')
  BRIGHTNESS_TARGET=$(( BRIGHTNESS - arg_stripped ))
else
#  echo else $MAX_BRIGHTNESS times $brite_arg divided by 100
  #BRIGHTNESS_TARGET=$(perl -le "printf '%2d', $MAX_BRIGHTNESS * ($brite_arg / 100)")
  #BRIGHTNESS_TARGET=$(echo "$MAX_BRIGHTNESS * $brite_arg / 100"|bc)
  BRIGHTNESS_TARGET=$(( MAX_BRIGHTNESS * brite_arg / 100 ))
fi
if [[ $BRIGHTNESS_TARGET -gt $MAX_BRIGHTNESS ]]; then
  BRIGHTNESS_TARGET=$MAX_BRIGHTNESS
  echo -n "Setting brightness to $MAX_BRIGHTNESS which is maximum brightness "
elif [[ $BRIGHTNESS_TARGET -lt $MIN_BRIGHTNESS ]]; then
  BRIGHTNESS_TARGET=$MIN_BRIGHTNESS
  echo -n "Setting brightness to $MIN_BRIGHTNESS which is minimum brightness "
elif [[ $BRIGHTNESS_TARGET -gt $BRIGHTNESS ]]; then
  echo -n "Increasing brightness to ${brite_arg}% resulting in "
elif [[ $BRIGHTNESS_TARGET -lt $BRIGHTNESS ]]; then
  echo -n "Reducing brightness to ${brite_arg}% resulting in "
elif [[ $BRIGHTNESS_TARGET -eq $BRIGHTNESS ]]; then
  echo "No change in brightness exiting."
  exit 0
fi
#echo $BRIGHTNESS_TARGET
set_brite
