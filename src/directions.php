<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="initial-scale=1,maximum-scale=1,user-scalable=no" />
  <title>RouteTask | Sample | ArcGIS API for JavaScript 4.18</title>
  <style>
    html,
    body,
    #viewDiv {
      padding: 0;

height: 100%;
width: 100%;

}

#paneDiv {
  position: absolute;
  height: 40%;
  top: 15px;
  left: 56px;
  padding: 0 12px 0 12px;
  background-color: rgba(0, 0, 0, 0.65);
  color: white;
  border-radius: 8px;

}
.press{
  font-family: sans-serif;
  text-align: center;
  position: fixed;
}

</style>

<link rel="stylesheet" href="https://js.arcgis.com/4.18/esri/themes/light/main.css" />
<script src="https://js.arcgis.com/4.18/"></script>

<script>


  require([
    "esri/Map",
    "esri/views/MapView",
    "esri/Graphic",
    "esri/layers/GraphicsLayer",
    "esri/tasks/RouteTask",
    "esri/tasks/support/RouteParameters",
    "esri/tasks/support/FeatureSet", 
    "esri/layers/GeoJSONLayer",
    "esri/renderers/UniqueValueRenderer",
    "esri/layers/CSVLayer",
    "esri/Color",
    "esri/symbols/SimpleLineSymbol",
    "esri/layers/KMLLayer",
    "dojo/parser", 
    "dojo/dom-style",
    "dijit/layout/BorderContainer", 
    "dijit/layout/ContentPane", 
    "dojo/domReady!",
    "esri/PopupTemplate"
    ], function(Map, MapView, Graphic, GraphicsLayer, RouteTask, RouteParameters, FeatureSet, GeoJSONLayer, UniqueValueRenderer, CSVLayer, Color, SimpleLineSymbol, KMLLayer, parser, domStyle, PopupTemplate) {


      const renderer = {
  type: "simple",  // autocasts as new SimpleRenderer()
  symbol: {
    type: "simple-line",  // autocasts as new SimpleMarkerSymbol()
    size: 6,
    color: "red",
    outline: {  // autocasts as new SimpleLineSymbol()
      width: 0.5,
      color: "blue"
    }
  }
};

let popupTemplate = new PopupTemplate({
  // autocasts as new PopupTemplate()
  title: "Route is in zip code: {ZIP_L}",
  outFields: ["*"]
});

const geoJSONLayer = new GeoJSONLayer({
 url: "https://localhost/test.geojson",
 copyright: "City of Los Angeles",
 renderer: renderer
});

geoJSONLayer.popupTemplate = popupTemplate;

geoJSONLayer.popupTemplate.content = "<p>ASSETID is <b>{ASSETID}</b>" +
"<ul><li>Street name is {STNAME}</li>" +
"<li>Old street is {OLD_STREET}</li><ul>";





var map = new Map({
  basemap: "streets-navigation-vector",
          layers: [geoJSONLayer] // Add the route layer to the map
        });


var view = new MapView({
          container: "viewDiv", // Reference to the scene div created in step 5
          map: map, // Reference to the map object created before the scene
          center: [-118.3433877, 34.0340639],
          zoom: 13
        });

});


  
  function enable() {
    document.getElementById("f").checked= true;

  }

  function disable() {
   document.getElementById("f").checked= false;
 }

 function refreshInput(input, val)
 {
  let txt="";

  if(val==0)
    txt="L";
  else if (val==1)
    txt="M";
  else 
    txt="H";

  if(input=="Debris")//debris
  {
    document.getElementById("Debris").innerHTML="Debris Factor: "+txt;


  }
  if(input=="Socio")//socioeconomic
  {

    document.getElementById("Socio").innerHTML="Socioeconomic Factor: "+txt;

  }

  if(input=="Foliage"){
    if(val=="on")
    {      
      enable();
    }
    else 
    {
      disable();
    }
  }

}


function updateTextInput(val) {
  if(document.getElementById("d").value==val)
   {   if(val==0)
    txt="L";
    else if (val==1)
      txt="M";
    else 
      txt="H";
    document.getElementById("Debris").innerHTML="Debris Factor: "+txt;
  }
  if(document.getElementById("s").value==val)
    {      if(val==0)
      txt="L";
      else if (val==1)
        txt="M";
      else 
        txt="H";
      document.getElementById("Socio").innerHTML="Socioeconomic Factor: "+txt;
    }
    if(document.getElementById("f").value=="on")
    {      
      enable();
    }
    else 
    {
      disable();
    }
    
  }



</script>
</head>
<body>


  <?php 
   //var_dump($_POST);
  if(isset($_POST['d']) &&
   isset($_POST['s']) &&
   isset($_POST['f']))
  {
    $val1 = $_POST['d'];
    $val2 = $_POST['s'];
    $val3 = $_POST['f'];
      // $exec_cmd = "../'Route Generation'/RouteCalculation {$num1} {$num2} {$num3}";
      // $json_result = shell_exec($exec_cmd);
  } else {
    $val1 = 0;
    $val2 = 0;
    $val3 = 0;
  }
  ?>

  <div id="viewDiv"></div>
  <div id="paneDiv" class="esri-widget">
    <br>
    <h3>Adjust the scales <br> to modify routes:</h3>
    <br><br>

    <form action="directions.php" id="form" method="POST">
      <!-- action="directions.php" -->
      <label for="customRange3" id="Debris" class="form-label">Debris Factor</label>
      <br>

      <input type="range" class="form-range" id="d" name="d" min="0" max="2" step="1" value="<?php echo $val1; ?>" onchange="updateTextInput(this.value);">
      <br>
      <label for="customRange3" id="Socio" class="form-label">Socioeconomic Factor</label>
      <br>
      <input type="range" class="form-range" id="s" name="s" min="0" max="2" step="1" value="<?php echo $val2; ?>" onchange="updateTextInput(this.value);">
      <br>
      <div class="form-check">
        <label class="form-check-label" id="Foliage"  for="flexCheckDefault">
          Tree Foliage Factor
        </label>
        <input class="form-check-input" id="f" name="f" type="checkbox" onchange="updateTextInput(this.value);" >
      </div>
      <br>
      <div class="container-fluid press">
        <button type="submit" class="btn btn-dark">Generate</button>
      </div>

<!--       <input type="file" id="files"  class="form-control" accept=".csv" required> 
-->    </div>

</form>
<div>
</div>
</div>
</body>
<script>

  let debris=<?php echo $val1; ?>;
  let socio=<?php echo $val2; ?>;
  let foliage="<?php echo $val3; ?>";
refreshInput("Debris", debris);
refreshInput("Socio", socio);
refreshInput("Foliage", foliage);
//alert(foliage);
</script>
</html>


