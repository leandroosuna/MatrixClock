leds = new Array(85)

function initTable()
{
    table = document.getElementById("led mx");
    
    
    var mirror = false;
    var ledC = 0;
    for(var y = 0; y < 5; y++)
    {
        var row = table.insertRow(-1);

        for(var x = 0; x < 17; x++)
        {
            
            var cell = row.insertCell(-1);
             
            var id = 0;
            
            id = ledC;
            
            cell.id = "led-"+id;
            cell.innerHTML = id;
            cell.className = "td-led";
            
            

            if(x<16)
                ledC++;
        }
        if(mirror)
        {
            var cells = Array.from(row.cells);

            // Sort the array
            cells.sort(function(a, b) {
                var aValue = a.id;
                var bValue = b.id;
                return aValue.localeCompare(bValue);
            });

            // Mirror the array
            cells.reverse();

            // Append the cells back to the row
            for (var i = 0; i < cells.length; i++) {
                row.appendChild(cells[i]);
            }
        }

        var cells = Array.from(row.cells);

        cells.forEach(cell =>{
            var cellid = cell.id;
            cell.onclick = (function(cellid) {
                return function() {
                    console.log("This is cell " + cellid);
                };
            })(cellid);
        }
        
        );
        mirror = !mirror;
        ledC++;
    }
    
}

function setLED(index, r,g,b)
{
    var led = document.getElementById("led-"+index);
    
    led.style.backgroundColor = 'rgb('+r+','+g+','+b+')';
 
}

function sendText()
{
    var xhttp = new XMLHttpRequest();
    var url = "set";
    var text = document.getElementById("textbox").value;
    var params = "text="+text;

    
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            console.log(this.responseText);
        }
    };
    xhttp.open("GET", url + "?" + params, true);
    
    xhttp.send();
}
var forced = false;
function updateMX()
{
    var xhttp = new XMLHttpRequest();
    var url = "mx-update";
    
    
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            
            if(this.responseText != "null")
            {
                forced = true; //forced first update
                var jsonData = JSON.parse(this.response);
            
                for (var i = 0; i < jsonData.length; i++) {
                    var led = jsonData[i].led;
                    var r = jsonData[i].red;
                    var g = jsonData[i].green;
                    var b = jsonData[i].blue;
                    
                    setLED(led,r,g,b);
                }
            }
            
        }
    };
    if(!forced)
        xhttp.open("GET", url+"-force", true);
    else
        xhttp.open("GET", url, true);
    
    xhttp.send();
}
setInterval(updateMX, 500);