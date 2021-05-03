<!doctype html>
<html lang="en">
<head>
  <!-- Required meta tags -->
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">

  <!-- Bootstrap CSS -->
  <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.0-beta3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-eOJMYsd53ii+scO/bJGFsiCZc+5NDVN2yr8+0RDqr0Ql0h+rP48ckxlpbzKgwra6" crossorigin="anonymous">

  <link rel="stylesheet" href="/resources/css/index.css">

  <title>Los Angeles City Street Sweeping Project</title>

</head>
<body>
  <video autoplay muted loop id="myVideo">
    <source src="http://lacity.org/sites/g/files/wph1706/files/2021-02/EchoPark_1920x800_loop.mp4" type="video/mp4">
      Your browser does not support HTML5 video.
    </video>

    <div class="container-fluid">
      <h1>Los Angeles City Street Sweeping Project</h1>
      <div class="fill">
        <div class="options">
          <form action="sweepingneed.php" id="form" method="POST">

            <label for="customRange3" id="Debris" class="form-label">Debris Factor</label>
            <input type="range" class="form-range" id="d" name="d" min="0" max="2" step="1" onchange="updateTextInput(this.value);">

            <label for="customRange3" id="Socio" class="form-label">Socioeconomic Factor</label>
            <input type="range" class="form-range" id="s" name="s" min="0" max="2" step="1" onchange="updateTextInput(this.value);">

            <div class="form-check">
              <label class="form-check-label"  for="flexCheckDefault">
                Tree Foliage Factor 
              </label>
              <input type="checkbox" id="f" name="f" onchange="updateTextInput(this.value);" >

            </div>
          </div>
          <div class="press">
            <button type="submit" class="btn btn-dark">Generate</button>
          </div>

        </form>
      </div>
    </div>

    <script src="/resources/js/index.js"></script>

  </body>
  </html>