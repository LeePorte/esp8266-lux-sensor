const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<style type="text/css">
.text {
    font-family: Arial, Helvetica, sans-serif;
}
</style>
<body style="background-color: #ffffff ">
<center>
<div>
<h1 class="text">Lux sensor to MQTT</h1>
</div>
 <br>
<div><h2 class="text">
  Lux Value: <span id="lux_val">0</span>
</h2>
</div>
<script>
setInterval(function()
{
  getData();
}, 2000);
function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("lux_val").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "luxread", true);
  xhttp.send();
}
</script>
</center>
</body>
</html>
)=====";
