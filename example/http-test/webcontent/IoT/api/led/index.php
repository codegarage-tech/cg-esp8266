<!DOCTYPE html>
<html lang="en">
   <head>
	  <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
      <meta name="Vikkey" content="Vivek Gupta & IoTMonk">
	  <meta http-equiv="Access-Control-Allow-Origin" content="*">
     
      <!-- If you are opening this page from local machine, uncomment belwo line 
       
	  <script src="http://ajax.googleapis.com/ajax/libs/jquery/2.0.0/jquery.min.js"></script> 
	  -->
	 
	 <!-- If you are opening this page from a web hosting server machine, uncomment belwo line -->
	 
	 <script type="text/javascript">
		document.write([
			"\<script src='",
			("https:" == document.location.protocol) ? "https://" : "http://",
			"ajax.googleapis.com/ajax/libs/jquery/1.2.6/jquery.min.js' type='text/javascript'>\<\/script>" 
		].join(''));
	  </script>
	  

      <title>ESP 8266 Node MCU Web APP LED Control IoT Monk</title>
	 <style>
	.center { 
		height: 400px;
		width: 400px;
		background: #BBDEFB;
		position: fixed;
		top: 50%;
		left: 50%;
		margin-top: -170px;
		margin-left: -200px;
	}
	
	.form{
		padding-top: 50px;
		padding-right: 30px;
		padding-bottom: 50px;
		padding-left: 30px;
	}

	.button {
		background-color: #4CAF50; /* Green */
		border: none;
		color: white;
		padding: 16px 32px;
		text-align: center;
		text-decoration: none;
		display: inline-block;
		font-size: 16px;
		margin: 4px 2px;
		-webkit-transition-duration: 0.4s; /* Safari */
		transition-duration: 0.4s;
		cursor: pointer;
	}

	.button1 {
		background-color: white; 
		color: black; 
	}

	.button1:hover {
		background-color: #4CAF50;
		color: white;
	}

	.button3 {
		background-color: white; 
		color: black; 
	}

	.button3:hover {
		background-color: #F44336;
		color: white;
	}

	.ip{
		background-color: #fffff; /* Green */
		border: none;
		color: black;
		padding: 16px 32px;
		text-align: center;
		text-decoration: none;
		display: inline-block;
		font-size: 16px;
		margin: 4px 2px;
		-webkit-transition-duration: 0.4s; /* Safari */
	}

	.footer{
		background:#64B5F6;
		width:100%;
		height:100px;
		position:absolute;
		bottom:0;
		left:0;
	}

	</style>
      
   </head>

<body bgcolor="#607D8B">
   <center>
		<h1 style="font-family: Helvetica;color: white;">ESP 12E NODEMCU CONTROLLER AND TESTER(Global)</h1>
	</center>
   
   <div class="center">
	<div align="center" class="form">
       <form action="" method="get">
              <button type="button" id="D1-on" class="button button1" >D1 ON</button>
			  <button type="button" id="D1-off" class="button button3" >D1 OFF</button>
			  <br>
			  <button type="button" id="D2-on" class="button button1" >D2 ON</button>
			  <button type="button" id="D2-off" class="button button3" >D2 OFF</button>
			  <br>
			  <button type="button" id="D3-on" class="button button1" >D3 ON</button>
			  <button type="button" id="D3-off" class="button button3" >D3 OFF</button>
			  <br>
        </form>
		<br><br>
	 </div>
	</div>
	
	<footer class="footer">
		<center>
			<h4 style="font-family: Helvetica;color: white;">&copy; 2017 | <a href="http://vsgupta.in/">Vivek Gupta</a> | <a href="http://www.iotmonk.com/">IoTMonk.com</a> </h4>
		</center>
	</footer>

    </body>
    
	<script>
		document.getElementById('D1-on').addEventListener('click', function() {
				var url = "https://iot-cuddle.000webhostapp.com/iot/api/led/update.php?id=1&status=on";
				$.getJSON(url, function(data) {
					console.log(data);
				});
		});
		
		document.getElementById('D1-off').addEventListener('click', function() {
				var url = "https://iot-cuddle.000webhostapp.com/iot/api/led/update.php?id=1&status=off";
				$.getJSON(url, function(data) {
					console.log(data);
				});
		});
		
		
		document.getElementById('D2-on').addEventListener('click', function() {
				var url = "https://iot-cuddle.000webhostapp.com/iot/api/led/update.php?id=2&status=on";
				$.getJSON(url, function(data) {
					console.log(data);
				});
		});
		
		document.getElementById('D2-off').addEventListener('click', function() {
				var url = "https://iot-cuddle.000webhostapp.com/iot/api/led/update.php?id=2&status=off";
				$.getJSON(url, function(data) {
					console.log(data);
				});
		});
		
		
		document.getElementById('D3-on').addEventListener('click', function() {
				var url = "https://iot-cuddle.000webhostapp.com/iot/api/led/update.php?id=3&status=on";
				$.getJSON(url, function(data) {
					console.log(data);
				});
		});
		
		document.getElementById('D3-off').addEventListener('click', function() {
				var url = "https://iot-cuddle.000webhostapp.com/iot/api/led/update.php?id=3&status=off";
				$.getJSON(url, function(data) {
					console.log(data);
				});
		});

		
	</script>
</html>
