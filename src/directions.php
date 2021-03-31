<!doctype html>
<html lang="en">
<head>
  <!-- Required meta tags -->
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">

  <!-- Bootstrap CSS -->
  <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.0-beta3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-eOJMYsd53ii+scO/bJGFsiCZc+5NDVN2yr8+0RDqr0Ql0h+rP48ckxlpbzKgwra6" crossorigin="anonymous">
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/2.1.3/jquery.min.js"></script>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/jquery-csv/0.71/jquery.csv-0.71.min.js"></script>
  <script src="https://code.jquery.com/jquery-3.3.1.min.js"></script>
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/2.1.3/jquery.min.js"></script>
  <script src="papaparse.js"></script>
  <style>
    .container-fluid {
      max-width: 30%;
    }
    .fill{
      background-color:rgba(0,0,0,0.6)!important; 
      padding: 15px;
    }
    .press, h1{
      font-family: sans-serif;
      text-align: center;
    }
    body{
      width: 100%;
      color: black;
      background: url('LA.jpg') no-repeat center center fixed; 
      -webkit-background-size: cover;
      -moz-background-size: cover;
      -o-background-size: cover;
      background-size: cover;
      z-index: -1;
      font-family: sans-serif;
    }


    label{

      font-size:20px;
      color: white;
    }


  </style>

  <title>Los Angeles City Street Sweeping Project</title>
 <?php echo var_dump($_POST); ?> 

</head>
<body>
  <br><br>
  <h1>Los Angeles City Street Sweeping Project</h1>
  <br><br><br><br>
  <div class="container-fluid fill">
 <div id="parsed_csv_list">

  </div>
  </div>
  <br>


 
  <script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.9.1/dist/umd/popper.min.js" integrity="sha384-SR1sx49pcuLnqZUnnPwx6FCym0wLsk5JZuNx2bPPENzswTNFaQU1RDvt3wT4gWFG" crossorigin="anonymous"></script>
  <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.0.0-beta3/dist/js/bootstrap.min.js" integrity="sha384-j0CNLUeiqtyaRmlzUHCPZ+Gy5fQu0dQ6eZ/xAww941Ai1SxSY+0EQqNXNE6DZiVc" crossorigin="anonymous"></script>

  <script>

let routes = <?$_POST["routes"]?>;
console.log(routes);

    function displayHTMLTable(routes){
      var table = "<table class='table'>";
      for(i=0;i<routes.size;i++){
        table+= "<tr>";
        console.log(routes.values().next().value);
        for(j=0;j<routes.values().next().value.length;j++){
          table+= "<td>";
          table+= routes.values().next().value[j];
          table+= "</th>";
        }
        table+= "</tr>";
      }
      table+= "</table>";
      $("#parsed_csv_list").html(table);
    }


displayHTMLTable(routes);




</script>

</body>
</html>