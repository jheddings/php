<?
  $type = $HTTP_GET_VARS["type"];
  if ($type) {
    header("Content-type: text/plain");

    if ($type == "hexstamp") {
      echo strtoupper(sprintf("%x", time()));
    } else if ($type == "guid") {
			$guid = md5(uniqid(rand(), true));
			printf("{%s-%s-%s-%s-%s}",
			substr($guid, 0, 8),
			substr($guid, 8, 4),
			substr($guid, 12, 4),
			substr($guid, 16, 4),
			substr($guid, 20, 12));
    } else {
      echo "INVALID ID TYPE";
    }
  } else {
?>

<html>
<head>
  <title>ID Generator</title>
</head>

<body>
<ul>
  <li><a href="idgen.php?type=guid">GUID / UUID</a></li>
  <li><a href="idgen.php?type=hexstamp">Current Timestamp in HEX</a></li>
</ul>
<i>Version: <b>1.1</b></i>
</body>
</html>

<? } ?>
