
var routes = { };

//debris, income, foliage

routes[[0, 2, "on"]] = "https://localhost/resources/data/geojson/LowDebrisHighIncome.geojson";
routes[[0, 2, 0]] = "https://localhost/resources/data/geojson/LowDebrisHighIncomeNoTrees.geojson";

routes[[2, 0, "on"]] = "https://localhost/resources/data/geojson/HighDebrisLowIncome.geojson";
routes[[2, 0, 0]] = "https://localhost/resources/data/geojson/HighDebrisLowIncomeNoTrees.geojson";

routes[[0, 1, "on"]] = "https://localhost/resources/data/geojson/LowDebris.geojson";
routes[[0, 1, 0]] = "https://localhost/resources/data/geojson/LowDebrisNoTrees.geojson";

routes[[1, 0, "on"]] = "https://localhost/resources/data/geojson/LowIncome.geojson";
routes[[1, 0, 0]] = "https://localhost/resources/data/geojson/LowIncomeNoTrees.geojson";
//debris income tree
routes[[2, 1, "on"]] = "https://localhost/resources/data/geojson/HighDebris.geojson";//data/geojson/debrisHigh.geojson";
routes[[2, 1, 0]] = "https://localhost/resources/data/geojson/HighDebrisNoTrees.geojson";

routes[[1, 2, "on"]] = "https://localhost/resources/data/geojson/HighIncome.geojson";
routes[[1, 2, 0]] = "https://localhost/resources/data/geojson/HighIncomeNoTrees.geojson";

routes[[0, 0, "on"]] = "https://localhost/resources/data/geojson/LowDebrisLowIncome.geojson";
routes[[0, 0, 0]] = "https://localhost/resources/data/geojson/LowDebrisLowIncomeNoTrees.geojson";

routes[[1, 1, "on"]] = "https://localhost/resources/data/geojson/Standard.geojson";
routes[[1, 1, 0]] = "https://localhost/resources/data/geojson/NoTrees.geojson";

routes[[2, 2, "on"]] = "https://localhost/resources/data/geojson/HighDebrisHighIncome.geojson";//output.geojson";//geojson/debrisHighincomeHigh.geojson

routes[[2, 2, 0]] = "https://localhost/resources/data/geojson/HighDebrisHighIncomeNoTrees.geojson";

//Debug
// console.log(debris);
// console.log(socio);
// console.log(foliage);
// alert(routes[[debris, socio, foliage]]);

require([
  "esri/Map",
  "esri/views/MapView",
  "esri/Graphic",
  "esri/layers/GraphicsLayer", 
  "esri/tasks/support/FeatureSet", 
  "esri/layers/GeoJSONLayer",
  "esri/renderers/UniqueValueRenderer",
  "esri/layers/CSVLayer",
  "esri/symbols/SimpleLineSymbol",
  "dojo/parser", 
  "dojo/dom-style",
  "esri/PopupTemplate",
  "esri/widgets/Search",
  "esri/smartMapping/statistics/uniqueValues",
  "esri/symbols/SimpleFillSymbol",
  "esri/Color",
  "esri/renderers/ClassBreaksRenderer",
  "esri/widgets/Legend",
  "dojo/domReady!"
  ], function(Map, MapView, Graphic, GraphicsLayer,  FeatureSet, GeoJSONLayer, UniqueValueRenderer, CSVLayer, SimpleLineSymbol, parser, domStyle, PopupTemplate, Search, uniqueValues, SimpleFillSymbol, Color, ClassBreaksRenderer, Legend) {



    const defaultSym = {
      type: "unique-value", 
      size: 3
    };

    const more75 = {
      type: "simple-line", 
      color: [56, 168, 0, 1],
      style: "solid",
      outline: {
        width: 0.2,
        color: [56, 168, 0, 1]
      }
    };

    const more50 = {
      type: "simple-line", 
      color: [139, 209, 0, 1],
      style: "solid",
      outline: {
        width: 0.2,
        color: [139, 209, 0, 1]
      }
    };

    const less50 = {
      type: "simple-line", 
      color: [  255, 165, 0, 1],
      style: "solid",
      outline: {
        width: 0.2,
        color: [  255, 165, 0, 1]
      }
    };

    const less35 = {
      type: "simple-line", 
      color: [255, 0, 0,1 ],
      style: "solid",
      outline: {
        width: 0.2,
        color: [255, 0, 0, 1]
      }
    };


    const renderer = {
      type: "class-breaks",
      field: "route",
      legendOptions: {
        title: "Routes:"
      },
      defaultSymbol: {
        type: "simple-line", 
        color: "black",
        style: "solid",
        outline: {
          width: 0.5,
          color: [50, 50, 50, 0.6]
        }
      },
      defaultLabel: "no data",
      classBreakInfos: [
      {
        minValue: 1,
        maxValue: 340,
        symbol: less35,
        label: "Most need for frequent sweeping"
      },
      {
        minValue: 341,
        maxValue: 700,
        symbol: less50,
        label: "More than average need for frequent sweeping"
      },
      {
        minValue: 491,
        maxValue: 740,
        symbol: more50,
        label: "Less than average need for frequent sweeping"
      },
      {
        minValue: 741,
        maxValue: 2000,
        symbol: more75,
        label: "Least need for frequent sweeping"
      }
      ]
    };


    let popupTemplate = new PopupTemplate({
      title: "<p>Index of need for route to be swept is <b>{route}</b>",
      outFields: ["*"]
    });

    const geoJSONLayer = new GeoJSONLayer({
     url: routes[[debris, socio, foliage]],
     copyright: "City of Los Angeles",
     renderer: renderer,
     title: "Suggested Route Sweeping Frequencies:"
   });

    geoJSONLayer.popupTemplate = popupTemplate;

    geoJSONLayer.popupTemplate.content = "lower index = higher need for route to be swept frequently.\n\n"+
    "<p>ASSETID is <b>{ASSETID}</b>";



    var map = new Map({
      basemap: "streets-navigation-vector",
      layers: [geoJSONLayer] 
    });


    var view = new MapView({
      container: "viewDiv", 
      map: map, 
      center: [-118.3433877, 34.0340639],
      zoom: 9.5
    });

    var searchWidget = new Search({
      view: view
    });

    view.ui.add(searchWidget, {
      position: "top-right",
      index: 2
    });


    const legend = new Legend({
      view: view
    });

    view.ui.add(legend, "bottom-left");

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

  if(input=="Foliage")//foliage
  {
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
  {   
    if(val==0)
      txt="L";
    else if (val==1)
      txt="M";
    else 
      txt="H";

    document.getElementById("Debris").innerHTML="Debris Factor: "+txt;
  }
  if(document.getElementById("s").value==val)
  {      
    if(val==0)
      txt="L";
    else if (val==1)
      txt="M";
    else 
      txt="H";
    document.getElementById("Socio").innerHTML="Socioeconomic Factor: "+txt;
  }

}
