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
      border-radius: 8px;
      width: 30%;
      position: fixed;
      top: 40%;
      left: 50%;
/*  bottom: 0;
*/  
color: #f1f1f1;
padding: 20px;
margin-top: -100px; /* Negative half of height. */
margin-left: -250px; /* Negative half of width. */
}
.fill{
/*      background-color:rgba(0,0,0,0.6)!important; 
*/     
position: fixed;
padding: 15px;
background: rgba(0, 0, 0, 0.5);
}

.press, h1{
  font-family: sans-serif;
  text-align: center;
  position: fixed;
}
h1{
  left: 22%;
}
.press{
  top: 63%;

}

#myVideo {
  position: fixed;
  right: 0;
  bottom: 0;
  min-width: 100%; 
  min-height: 100%;
}

.form-check{
  text-align: center;
}
#f{
padding-left: 3px;
}
body{
  width: 100%;
  color: black;
  margin: 0;
  font-size: 17px;
  font-family: sans-serif;
}

</div>

label{
  font-size:20px;
  color: white;
}


</style>

<title>Los Angeles City Street Sweeping Project</title>

</head>
<body>
  <video autoplay muted loop id="myVideo">
    <source src="http://lacity.org/sites/g/files/wph1706/files/2021-02/EchoPark_1920x800_loop.mp4" type="video/mp4">
      Your browser does not support HTML5 video.
    </video>

    <br><br>
    <h1>Los Angeles City Street Sweeping Project</h1>
    <br><br><br><br>
    <div class="container-fluid fill">

      <form action="directions.php" id="form" method="POST">
        <!-- action="directions.php" -->
        <label for="customRange3" id="Debris" class="form-label">Debris Factor</label>
        <input type="range" class="form-range" id="d" name="d" min="0" max="2" step="1" value="<?php echo $val1; ?>" onchange="updateTextInput(this.value);">

        <label for="customRange3" id="Socio" class="form-label">Socioeconomic Factor</label>
        <input type="range" class="form-range" id="s" name="s" min="0" max="2" step="1" value="<?php echo $val2; ?>" onchange="updateTextInput(this.value);">

        <div class="form-check">
          <label class="form-check-label"  for="flexCheckDefault">
            Tree Foliage Factor 
          </label>
          <input type="checkbox" id="f" name="f" onchange="updateTextInput(this.value);" >

        </div>
<div class="container-fluid press">
  <button type="submit" class="btn btn-dark">Generate</button>
</div>
   </div>

</form>
</div>

<?php if(isset($json_result)): ?>
  <h3>JSON result from executable</h3>
  <pre><?php echo $json_result; ?></pre>
<?php endif; ?>

<!-- <div id="parsed_csv_list">

</div> -->
<script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.9.1/dist/umd/popper.min.js" integrity="sha384-SR1sx49pcuLnqZUnnPwx6FCym0wLsk5JZuNx2bPPENzswTNFaQU1RDvt3wT4gWFG" crossorigin="anonymous"></script>
<script src="https://cdn.jsdelivr.net/npm/bootstrap@5.0.0-beta3/dist/js/bootstrap.min.js" integrity="sha384-j0CNLUeiqtyaRmlzUHCPZ+Gy5fQu0dQ6eZ/xAww941Ai1SxSY+0EQqNXNE6DZiVc" crossorigin="anonymous"></script>

<script>
  var txt="Low";
  document.getElementById("d").value=0;
  document.getElementById("s").value=0;
  document.getElementById("Debris").innerHTML="Debris Factor: "+txt;
  document.getElementById("Socio").innerHTML="Socioeconomic Factor: "+txt;

  const inputs = document.getElementById('files');

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



  function printCSV(a)
  {
   let route=-1, segment=-1, order=-1;
   let init_route=-1;
   let routes = new Map();
      //routes.set(a[i][j],
      for(let j=0; j<a[0].length; j++)
        { if(a[0][j]== "ROUTE")
      {
       route=j;
       if(a[1][route]!= null)
         init_route=a[1][route];
     }
     else
      if(a[0][j]=="ASSETID")//segment id
       segment=j;
     else
      if(a[0][j]=="ORDER")
       order=j;
   }
   let segments=[];

   for(let i=1; i<a.length; i++) {
    if(a[i][route]==init_route){
      segments[a[i][order]]=a[i][segment];
    }
    else
    {
      routes.set(a[i][route], segments);
      init_route=a[i][route];
      segments=[];
    }

  }

  console.log(routes);
  var x = document.createElement("input");
// x.setAttribute("type", "hidden");
x.setAttribute("name", "routes");
x.setAttribute("style", "display: none;");

// x.style.visibility="hidden";
// x.style.display="none";
// x.value=routes;
// document.getElementById('form').innerHTML+=x;

let jsonObject = JSON.stringify(a);  
// routes.forEach((value, key) => {  
//     jsonObject[key] = value  
// }); 

//   x.value = JSON.stringify(jsonObject);

x.value=jsonObject;

console.log(x.value);
document.getElementById('form').appendChild(x);
}



inputs.addEventListener('change', e => {
  const fileList = inputs.files;
  const theFile = fileList[0];
  Papa.parse(theFile, {
    complete: function(results) {
    //console.log("Finished:", results);
    printCSV(results.data);
  }
});
});


function updateTextInput(val) {
  if(document.getElementById("d").value==val)
   {   if(val==0)
    txt="Low";
    else if (val==1)
      txt="Medium";
    else 
      txt="High";
    document.getElementById("Debris").innerHTML="Debris Factor: "+txt;
  }
  if(document.getElementById("s").value==val)
    {      if(val==0)
      txt="Low";
      else if (val==1)
        txt="Medium";
      else 
        txt="High";
      document.getElementById("Socio").innerHTML="Socioeconomic Factor: "+txt;
    }
    
  }


  function loadPage()
  {
   window.location="directions.php";

 }

 function fetchJSON(url) {
  return fetch(url)
  .then(function(response) {
    return response.json();
  });
}

var census_centerline = fetchJSON('https://opendata.arcgis.com/datasets/9ce7522ad9af44b8900e7c712eb424be_2.geojson')
.then(function(data) { 

        // do what you want to do with `data` here...
        census_centerline.features.forEach(function(feature) {
          // console.log(feature);
          // var symbol = feature.properties['OLD_STREET'];
          // console.log(symbol);
        });

      });

var centroid_routes = fetchJSON('https://opendata.arcgis.com/datasets/39dfa7b7b87c4027a650828007debb5a_0.geojson')
.then(function(data) { 

        // do what you want to do with `data` here...
        centroid_routes.features.forEach(function(feature) {
          // console.log(feature);
          // var symbol = feature.properties['OLD_STREET'];
          // console.log(symbol);
        });

      });
var trees_servicerequests = fetchJSON('https://opendata.arcgis.com/datasets/d47a911d5ef84beaa4f6a5a0e16e59dd_0.geojson')
.then(function(data) { 


  trees_servicerequests.features.forEach(function(feature) {
   // console.log(feature);
   // var symbol = feature.properties['OLD_STREET'];
   // console.log(symbol);
 });

});
var clean_streets_index = fetchJSON('https://opendata.arcgis.com/datasets/1cff88121e814257bd1047d869ce8e3c_0.geojson')
.then(function(data) { 

        // do what you want to do with `data` here...
        clean_streets_index.features.forEach(function(feature) {
         // console.log(feature);
          // var symbol = feature.properties['OLD_STREET'];
          // console.log(symbol);
        });

      });


    </script>

  </body>
  </html>