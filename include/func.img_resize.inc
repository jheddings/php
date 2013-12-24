<?php
////////////////////////////////////////////////////////////////////////////////
// Resizes an incomming image while maintaining the original aspect ratio.
//   im - the created image (as from imagecreate())
//   maxWidth - the maximum width of the saved result (or 0 for no max)
//   maxHeight - the maximum height of the saved result (or 0 for no max)
//   resample - if true, the image will be resampled, otherwise, it will
//     just be resized -- resampling takes longer, but usually produces
//     higher quality results
//   thumbnail - if true, the resulting image will be $maxheight x $maxwidth
//     with the resized image centered in the thumbnail
//
// Version: 1.2
//
function resize_image($img, $maxwidth, $maxheight, $resample = false, $thumbnail = false) {
  $width = imagesx($img);
  $height = imagesy($img);

  // determine what the ratio of the scaled image will be
  $wratio = ($maxwidth) ? $maxwidth / $width : 1;
  $hratio = ($maxheight) ? $maxheight / $height : 1;
  $ratio = ($wratio < $hratio) ? $wratio : $hratio;

  // and get the new dimensions
  $new_width = $width * $ratio;
  $new_height = $height * $ratio;

  // figure out the size of the new image
  $rwidth = ($thumbnail && $maxwidth) ? $maxwidth : $new_width;
  $rheight = ($thumbnail && $maxheight) ? $maxheight : $new_height;

  // now figure out the placement within the new image
  $new_x = ($rwidth - $new_width) / 2;
  $new_y = ($rheight - $new_height) / 2;

  // set up the image and transparency info if we are thumbnailing
  $rimg = imagecreatetruecolor($rwidth, $rheight);
  if ($thumbnail) {
    $back = imagecolorallocate($rimg, 250, 250, 250);  // arbitrary color
    imagefill($rimg, 0, 0, $back);
    imagecolortransparent($rimg, $back);
  }

  // do the resizing -- see if we can / should resample or resize
  if (($resample) && (function_exists("imagecopyresampled"))) {
    imagecopyresampled($rimg, $img, $new_x, $new_y, 0, 0,
                       $new_width, $new_height, $width, $height);
  } else {
    imagecopyresized($rimg, $img, $new_x, $new_y, 0, 0,
                     $new_width, $new_height, $width, $height);
  }

  return $rimg;
}
?>
