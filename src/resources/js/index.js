var txt="Low";
document.getElementById("d").value=0;
document.getElementById("s").value=0;
document.getElementById("Debris").innerHTML="Debris Factor: "+txt;
document.getElementById("Socio").innerHTML="Socioeconomic Factor: "+txt;

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