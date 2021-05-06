<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="initial-scale=1,maximum-scale=1,user-scalable=no" />
  <title>Los Angeles City Street Sweeping Project</title>

  <link rel="stylesheet" href="https://js.arcgis.com/4.18/esri/themes/dark/main.css" />
  <link rel="stylesheet" href="https://js.arcgis.com/3.36/esri/css/esri.css">

  <link rel="stylesheet" href="/resources/css/sweepingneed.css">

  <script src="https://js.arcgis.com/4.18/"></script>


</head>
<body>

  <?php 
  if(isset($_POST['d']) ||
   isset($_POST['s']))
  {
    $val1 = $_POST['d'];
    $val2 = $_POST['s'];
  }
  if(isset($_POST['f']))
  {
        $val3 = $_POST['f'];

  }
  else
  {
            $val3 = 0;

  }
  ?>


  <div id="viewDiv">
    <div id="paneDiv" class="esri-widget">
      <h3>Adjust the scales to modify routes:</h3>


      <form action="sweepingneed.php" id="form" method="POST">
        <label for="customRange3" id="Debris" class="form-label">Debris Factor</label>
        <input type="range" class="form-range" id="d" name="d" min="0" max="2" step="1" value="<?php echo $val1; ?>" onchange="updateTextInput(this.value);">

        <label for="customRange3" id="Socio" class="form-label">Socioeconomic Factor</label>
        <input type="range" class="form-range" id="s" name="s" min="0" max="2" step="1" value="<?php echo $val2; ?>" onchange="updateTextInput(this.value);">

        <div class="form-check">
          <label class="form-check-label" id="Foliage"  for="flexCheckDefault">
            Tree Foliage Factor:
          </label>
          <input class="form-check-input" id="f" name="f" type="checkbox" onchange="updateTextInput(this.value);">
        </div>
        
        <div class="press">
          <button type="submit" class="btn btn-dark">Generate</button>
        </div>
      </form>
    </div>
  </div>
</body>
<script>
  let debris="<?php echo $val1; ?>";
  let socio="<?php echo $val2; ?>";
  let foliage="<?php echo $val3; ?>";
</script>

<script src="/resources/js/sweepingneed.js"></script>

<script>
  refreshInput("Debris", debris);
  refreshInput("Socio", socio);
  refreshInput("Foliage", foliage);

</script>
</html>


