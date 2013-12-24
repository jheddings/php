<?
  session_start();
  require_once("captcha.inc");
?>
<html>
<head>
  <title>Captcha Test Page</title>
  <script language="javascript">
    function init() { captcha_new(); }

    function captcha_new() {
      var now = new Date();

      var sel = document.getElementById("which");
      var which = sel.options[sel.selectedIndex].value;

      var sel = document.getElementById("size");
      var size = sel.options[sel.selectedIndex].value;

      var url = "captcha.php?" + which + "&" + size + "&" + now.getTime();

      document.getElementById("captcha").src = url;
      document.getElementById("code").value = "";
      document.getElementById("code").focus();
    }
  </script>
</head>

<body onload="init();">
  <form action="index.php" method="post">
    <p><img src="" id="captcha" onclick="captcha_new();" /></p>

    <p>
      <input type="text" name="code" id="code" size="8" maxlength="6" />
      <input type="submit" name="do-check" id="do-check" value="Check" />
    </p>

    <p>
      <select id="size" onchange="captcha_new();">
        <option value="">Default</option>
        <option value="width=320&height=120">Medium</option>
        <option value="width=640&height=240">Big</option>
        <option value="width=1024&height=420">Huge</option>
      </select>

      <select id="which" onchange="captcha_new();">
        <option value="">Random</option>
        <option value="which=1">Dots &amp; Lines</option>
        <option value="which=2">Destroyed</option>
        <option value="which=3">Blotches</option>
      </select>
    </p>

  </form>

<?
  if (isset($_POST["do-check"])) {
    if (captcha_check($_POST["code"])) {
      printf("<font color=\"#00FF00\">CORRECT</font>");
    } else {
      printf("<font color=\"#FF0000\">INCORRECT</font>");
    }
  }
?>
</body>
</html>
