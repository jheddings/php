<?php
  session_start();
  require_once("captcha.inc");

  $length = (isset($_GET["length"])) ? $_GET["length"] : 6;
  $width = (isset($_GET["width"])) ? $_GET["width"] : 120;
  $height = (isset($_GET["height"])) ? $_GET["height"] : 40;
  $which = (isset($_GET["which"])) ? $_GET["which"] : 0;

  captcha($length, $width, $height, $which);
?>
