<!doctype html>
<html>
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
      width:600px; 
    overflow-x:scroll; 
    position:relative;
    height: 280px;
    }
    table{
      background-color:rgba(0,0,0,0.6)!important; 
      padding: 15px;
      text-align: center;
      overflow-x: scroll;
    }
    tbody {
   overflow-x: scroll;
}

th, td{
  min-width: 200px;
}
    tr, td{
      color: white;
    }
    .press, h1, h4{
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
<!--   <?php echo var_dump($_POST); ?> 
 -->
</head>
<body>
  <br><br>
  <h1>Los Angeles City Street Sweeping Project</h1>
  <h4>(Routes and segments in order to be followed)</h4>
  <br><br><br><br>
  <div class="container-fluid fill">
 <div id="parsed_csv_list">

  </div>
  </div>
  <br>


 
  <script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.9.1/dist/umd/popper.min.js" integrity="sha384-SR1sx49pcuLnqZUnnPwx6FCym0wLsk5JZuNx2bPPENzswTNFaQU1RDvt3wT4gWFG" crossorigin="anonymous"></script>
  <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.0.0-beta3/dist/js/bootstrap.min.js" integrity="sha384-j0CNLUeiqtyaRmlzUHCPZ+Gy5fQu0dQ6eZ/xAww941Ai1SxSY+0EQqNXNE6DZiVc" crossorigin="anonymous"></script>

  <script>
    
const geoJSONLayer = new GeoJSONLayer({
   url: "https://opendata.arcgis.com/datasets/a3f84024497b4aea8bc2c2ad6db77d70_3.geojson",
   copyright: "City of Los Angeles",
});
map.add(geoJSONLayer);  // adds the layer to the map

let routes = <?php echo $_POST["routes"]; ?>;
console.log(routes);
let size=Object.keys(routes).length;
    function displayHTMLTable(routes){
      var table = "<table class='table'>";

table+="<thead>";
        table+= "<tr>";
        table+="<th>";
        table+="route "+i+": ";
          table+= "</th>";
          table+="</tr>";
          table+="</thead>";
          table+="<tbody> <tr>";
        for(j=0;j<Object.keys(routes[i]).length; j++){
                  console.log(routes[i][j]);

          table+= "<td>";
          table+= routes[i][j];
          table+="</td>";
        }
        table+= "</tr>";
      }
      table+="</tbody>";
      table+= "</table>";
      $("#parsed_csv_list").html(table);
    }


displayHTMLTable(routes);




</script>

</body>
</html>