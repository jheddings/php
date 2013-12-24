<?php

  /*
  ** PHP Captcha Manager
  */

  define("CAPTCHA_SESS_PARAM", "captcha-code");
  define("CAPTCHA_COUNT", 3);

  define("CAPTCHA_CHARS_ALL", "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
  define("CAPTCHA_CHARS_EASY", "CDFGHJKLMNPRSTVWXYZ2345679");
  define("CAPTCHA_CHARS", CAPTCHA_CHARS_EASY);

  //////////////////////////////////////////////////////////////////////////////
  // create and display a new captcha, saving it to the session
  function captcha($length, $width, $height, $which = 0) {
    $_SESSION[CAPTCHA_SESS_PARAM] = captcha_generate($length);
    captcha_display($_SESSION[CAPTCHA_SESS_PARAM], $width, $height, $which);
  }

  //////////////////////////////////////////////////////////////////////////////
  // generate a good captcha of the specified size
  function captcha_generate($length) {
    $len = strlen(CAPTCHA_CHARS);
    for ($cnt = 0; $cnt < $length; $cnt++) {
      $code .= substr(CAPTCHA_CHARS, mt_rand(0, $len - 1), 1);
    }
    return strtoupper($code);
  }

  //////////////////////////////////////////////////////////////////////////////
  // check if the given captcha matches the saved value, resetting the captcha
  function captcha_check($code) {
    $ret = false;

    if (isset($_SESSION[CAPTCHA_SESS_PARAM])) {
      $code = strtoupper($code);
      $ret = ($code == $_SESSION[CAPTCHA_SESS_PARAM]);
      unset($_SESSION[CAPTCHA_SESS_PARAM]);
    }

    return $ret;
  }

  //////////////////////////////////////////////////////////////////////////////
  // verify the given code matches the stored captcha, or die with a message
  function captcha_verify($code, $msg = "invalid captcha") {
    captcha_check($code) or die($msg);
  }

  //////////////////////////////////////////////////////////////////////////////
  // display a specific captcha using the given width and height
  function captcha_display($code, $width, $height, $which = 0) {
    $image = @imagecreatetruecolor($width, $height) or die("imagecreate failed");
    imagefill($image, 0, 0, imagecolorallocate($image, 255, 255, 255));

    captcha_picka($image, $code, $which);

    // send some good headers
    header("Content-Type: image/png");
    header("Cache-Control: no-cache, must-revalidate");
    header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
    header("Content-Disposition: inline; filename=captcha.png");

    // send & cleanup
    imagepng($image);
    imagedestroy($image);
  }

  //////////////////////////////////////////////////////////////////////////////
  // generate random dots on the image
  function captcha_dots(&$image, $color = null) {
    $width = imagesx($image);
    $height = imagesy($image);

    $numdots = ($width * $height) / 4;
    for ($cnt = 0; $cnt < $numdots; $cnt++) {
      $x = mt_rand(0, $width);
      $y = mt_rand(0, $height);

      if ($color === null) {
        imagesetpixel($image, $x, $y, captcha_color($image));
      } else {
        imagesetpixel($image, $x, $y, $color);
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // generate random lines on the image
  function captcha_lines(&$image, $color = null, $numlines = 10) {
    $width = imagesx($image);
    $height = imagesy($image);

    // make lines 4% of the image height
    imagesetthickness($image, (max(2, 0.04*$height)));

    for ($cnt = 0; $cnt < $numlines; $cnt++) {
      $x1 = mt_rand(0, $width);
      $y1 = mt_rand(0, $height);
      $x2 = mt_rand(0, $width);
      $y2 = mt_rand(0, $height);

      if ($color === null) {
        imageline($image, $x1, $y1, $x2, $y2, captcha_color($image));
      } else {
        imageline($image, $x1, $y1, $x2, $y2, $color);
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // generate random arcs on the image
  function captcha_arcs(&$image, $color = null, $numarcs = 10) {
    $width = imagesx($image);
    $height = imagesy($image);

    // make lines 4% of the image height
    imagesetthickness($image, (max(2, 0.04*$height)));

    for ($cnt = 0; $cnt < $numarcs; $cnt++) {
      $cx = mt_rand(0, $width);
      $cy = mt_rand(0, $height);
      $w = mt_rand(0, $width);
      $h = mt_rand(0, $height);
      $sa = mt_rand(0, 90);
      $ea = mt_rand(0, 90);

      if ($color === null) {
        imagearc($image, $cx, $cy, $w, $h, $sa, $ea, captcha_color($image));
      } else {
        imagearc($image, $cx, $cy, $w, $h, $sa, $ea, $color);
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // add ellipses (blotches) to the image
  function captcha_blotch(&$image, $color = null) {
    $width = imagesx($image);
    $height = imagesy($image);

    for ($cnt = 0; $cnt < 100; $cnt++) {
      $x = mt_rand(0, $width);
      $y = mt_rand(0, $height);
      $w = mt_rand(0, $width);
      $h = mt_rand(0, $height);

      if ($color === null) {
        imagefilledellipse($image, $x, $y, $w, $h, captcha_color($image));
      } else {
        imagefilledellipse($image, $x, $y, $w, $h, $color);
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // add text to the image, with optional shadow
  function captcha_text(&$image, $text, $color = null, $font = null, $shadow = null) {
    $width = imagesx($image);
    $height = imagesy($image);

    $size = $height * 0.75;
    $angle = mt_rand(-10, 10);

    if ($font === null) { $font = captcha_font(); }
    if ($color === null) { $color = captcha_color($image, 50, 75); }

    $textbox = imagettfbbox($size, $angle, $font, $text) or die("imagettfbbox failed");

    $x = ($width - $textbox[4]) / 2;
    $y = ($height - $textbox[5]) / 2;

    if ($shadow !== null) {
      if ($shadow === true) { $shadow = captcha_color($image, 100, 200); }
      imagettftext($image, $size, $angle, $x + 2, $y + 2, $shadow, $font, $text) or die("imagettftext failed");
    }

    imagettftext($image, $size, $angle, $x, $y, $color, $font, $text) or die("imagettftext failed");
  }

  //////////////////////////////////////////////////////////////////////////////
  // overlays an image on top of the image
  function captcha_blend(&$image, &$overlay) {
    $width = imagesx($image);
    $height = imagesy($image);

    for ($y = 0; $y < $height; $y++) {
      for ($x = 0; $x < $width; $x++) {
        $ovrARGB = imagecolorat($overlay, $x, $y);
        $dstARGB = imagecolorat($image, $x, $y);

        $ovrR = $ovrARGB >> 16 & 0xFF;
        $ovrG = $ovrARGB >> 8 & 0xFF;
        $ovrB = $ovrARGB & 0xFF;
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // returns a "good" font to use in the image
  function captcha_font() {
    return "captcha.ttf";
  }

  //////////////////////////////////////////////////////////////////////////////
  // returns a "good" color for use in the image
  function captcha_color(&$image, $min = 75, $max = 125, $alpha = 0) {
    $r = mt_rand($min, $max);
    $g = mt_rand($min, $max);
    $b = mt_rand($min, $max);

    $color = imagecolorexactalpha($image, $r, $g, $b, $alpha);
    if ($color < 0) {
      $color = imagecolorallocatealpha($image, $r, $g, $b, $alpha);
    }

    return $color;
  }

  //////////////////////////////////////////////////////////////////////////////
  // randomly pick an run a captcha generator
  function captcha_picka(&$image, $code, $which = 0) {
    if ($which == 0) { $which = mt_rand(1, CAPTCHA_COUNT); }
    eval("captcha_0" . $which . '($image, $code);');
  }

  //////////////////////////////////////////////////////////////////////////////
  // a bunch of colorful noise with text on top
  function captcha_01(&$image, $code) {
    captcha_arcs($image);
    captcha_text($image, $code);
    captcha_lines($image);
    captcha_dots($image);
  }

  //////////////////////////////////////////////////////////////////////////////
  // black & white, noisy & destroyed text
  function captcha_02(&$image, $code) {
    $black = imagecolorallocate($image, 0, 0, 0);
    $white = imagecolorallocate($image, 255, 255, 255);

    captcha_text($image, $code, $black);
    captcha_dots($image, $white);
    captcha_dots($image, $black);
  }

  //////////////////////////////////////////////////////////////////////////////
  // blotches alpha-blended with text
  function captcha_03(&$image, $code) {
    imagefill($image, 0, 0, captcha_color($image));
    captcha_blotch($image);

    $color = captcha_color($image, 200, 255, 100);
    captcha_text($image, $code, $color);
  }

  //////////////////////////////////////////////////////////////////////////////
  // FILTERS
  $filter_emboss = array(array(2, 0, 0), array(0, -1, 0), array(0, 0, -1));
  $filter_gaussian = array(array(1, 2, 1), array(2, 4, 2), array(1, 2, 1));
?>
