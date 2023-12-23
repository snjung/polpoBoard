		// Declare and Initialize variables
		var teamAName="Team A"; //Name of Team A
		var teamBName="Team B"; //Name of Team B
		var teamAscore=0;  //Number of Goals for Team A
		var gameLength=7; //Standard duration of a game in Minutes
		var intervall=1; //Set the refresh interval for the html page in x100ms . Working and tested: 10 (update every second), 5 (update every .5s), 1 (update every .1s)
		var counter=gameLength*60*10; //Number of 1/10 seconds till the end of the game
		var teamBscore=0; //Number of Goals for Team B
		var teamTempscore=0; //Temporary stores score of one team when the halftime function is called
		var qtr=1; //1st or second halftime, currently not used, could be implemented later on
		var counteractual="0000"; //String that will hold the remaining time in the Format "mmss" for transfer to arduino
		var testmode=0; // Set test mode to disabled
		var timerInterval;
		var timerTrack;
		var secondsDisplayed=61; //Used to check if Scoreboard has to be updated. 
		
		/* ***** TIMING FUNCTIONS ****
		   *************************** */
		
		// Refreshes the timer display on the webpage, diminishes counter by 10ms and initiates an update to the hardware display
		function timerLoop() {
			if(counter==0) {
				$('#timer_count').html("00:00");
				$('#timer_count').css('animation','done .5s 5');
				clearInterval(timerInterval);
			} else {
				if(timerTrack==true) { counter=counter-intervall; }
				var minutes = Math.floor(counter/10 / 60);
				var seconds = Math.floor(counter/10 - minutes * 60);
				var tenths = counter-minutes*600-seconds*10;
				var finalTime = str_pad_left(minutes,'0',2)+':'+str_pad_left(seconds,'0',2)+'.'+tenths;
				counteractual = str_pad_left(minutes,'0',2)+''+str_pad_left(seconds,'0',2);
				$('#timer_count').html(finalTime);
			}
			if (seconds!=secondsDisplayed) {
				updateScoreboardNow();
				secondsDisplayed = seconds;
			}
		}

		// Deals with starting/stopping or setting the countdown timer
		function timer_change(action,value) {
			if(action=="start") {
				if(timerTrack==true) { 
					timerTrack=false; 
					clearInterval(timerInterval); 
					$('#timer_count').css('animation','stop .5s 2'); 
					} 
				else { 
					timerTrack=true; 
					timerInterval=setInterval(timerLoop, 100*intervall); //setInterval starts a clock that periodically calls the defined function
					}
			}			
			if(action=="set") {
				timerTrack=false; 
				clearInterval(timerInterval); //clearInterval stopps the timer initialized by setInterval
				$('#timer_count').css('animation','stop .5s 2');
				swal({
				  title: "Stoppuhr setzen",
				  text: "Eingabe: mm:ss oder mmss oder mm oder m",
				  type: "question",
				  input: 'text',
				  showCancelButton: true,
				  inputPlaceholder: "Time"
				}).then(function (result) {
					if(result == ""){
						return false;
					}
					if(result.length==1){
						counter=result*60*10;
						if(timerTrack==false) { timerLoop(); }
					}
					if(result.length==2){
						counter=result*60*10;
						if(timerTrack==false) { timerLoop(); }
					}
					if(result.length==4){
						var mins = result.substring(0, 2);
						var secs = result.substring(2, 4);
						counter=(parseInt(mins)*60*10)+parseInt(secs)*10;
						timerLoop();
					}	
					if(result.length==5){
						var partsOfTime = result.split(':');
						counter=(parseInt(partsOfTime[0])*60*10)+parseInt(partsOfTime[1]*10);
						timerLoop();
					}					
					if (result === "") {
						swal.showInputError("You need to enter something!");
						return false;
					}
					if (result.length>5) {
						return false;
				  }
				timerLoop();
								})
			}
			if(action=="set7") {
				timerTrack=false; 
				clearInterval(timerInterval); //clearInterval stopps the timer initialized by setInterval
				$('#timer_count').css('animation','stop .5s 2');
				  counter=420*10;if(timerTrack==false) { timerLoop(); }
				timerLoop();
				updateScoreboardNow();
			}
			if(action=="set9") {
				timerTrack=false; 
				clearInterval(timerInterval); //clearInterval stopps the timer initialized by setInterval
				$('#timer_count').css('animation','stop .5s 2');
				  counter=540*10;if(timerTrack==false) { timerLoop(); }
				timerLoop();
				updateScoreboardNow();
			}
			if(action=="set10") {
				timerTrack=false; 
				clearInterval(timerInterval); //clearInterval stopps the timer initialized by setInterval
				$('#timer_count').css('animation','stop .5s 2');
				counter=600*10;if(timerTrack==false) { timerLoop(); }
				timerLoop();
				updateScoreboardNow();
			}

		}
		
		//Called after website loading to get the proper time displayed on the countdown timer
		function initializeClock() {
				var minutes = Math.floor(counter/10 / 60);
				var seconds = Math.floor(counter/10 - minutes * 60);
				var tenths = counter-minutes*600-seconds*10;
				var finalTime = str_pad_left(minutes,'0',2)+':'+str_pad_left(seconds,'0',2)+'.'+tenths;
				counteractual = str_pad_left(minutes,'0',2)+''+str_pad_left(seconds,'0',2);
				$('#timer_count').html(finalTime);
		}
		
		//Takes a string and prepends the specified symbol to get a string with target length
		function str_pad_left(string,pad,length) {
			return (new Array(length+1).join(pad)+string).slice(-length);
		}
		

				
		function teamAscore_change(action,value) {
			if(action=="plus") {if(teamAscore+value>99) {showError("Punktestand kann nicht größer als 99 sein!");} else {teamAscore=teamAscore+value;updateScoreboardNow();}}
			if(action=="minus") {if(teamAscore-value<0) {showError("Punktestand kann nicht größer unter 0 sein!");} else {teamAscore=teamAscore-value;updateScoreboardNow();}}
			if(action=="zero") {
				swal({
				  title: 'Sicher?',
				  text: "Punkte Team A wirklich zuruecksetzen?",
				  type: 'warning',
				  showCancelButton: true,
				  confirmButtonColor: '#3085d6',
				  cancelButtonColor: '#d33',
				  confirmButtonText: 'Ja',
				  cancelButtonText: 'Nein'
				}).then(function () {
				  swal({
					title: 'Deleted!',
					text: 'Your file has been deleted.',
					type: 'success',
					timer: 300,
					showConfirmButton: false
				  });
				  teamAscore=0;
				  updateScoreboardNow();
				})
			}
		}		

		
		function teamBscore_change(action,value) {
			if(action=="plus") {if(teamBscore+value>99) {showError("Punktestand kann nicht größer als 99 sein!");} else {teamBscore=teamBscore+value;updateScoreboardNow();}}
			if(action=="minus") {if(teamBscore-value<0) {showError("Punktestand kann nicht größer unter 0 sein!");} else {teamBscore=teamBscore-value;updateScoreboardNow();}}
			if(action=="zero") {
				swal({
				  title: 'Sicher?',
				  text: "Punkte Team B wirklich zurücksetzen?",
				  type: 'warning',
				  showCancelButton: true,
				  confirmButtonColor: '#3085d6',
				  cancelButtonColor: '#d33',
				  confirmButtonText: 'Ja',
				  cancelButtonText: 'Nein'
				}).then(function () {
				  swal({
					title: 'Deleted!',
					text: 'Your file has been deleted.',
					type: 'success',
					timer: 300,
					showConfirmButton: false
				  });
				  teamBscore=0;
				  updateScoreboardNow();
				})
			}
		}			
		
		function updateScoreboardNow() {
			$('#teamAscore_count').html(pad(teamAscore,2));
			$('#teamBscore_count').html(pad(teamBscore,2));
			submitForm();
		}
		
		function showError(errormsg) {
			swal("Not Possible", errormsg, "error");
		}
		
		function halfTime() {
			teamAName=document.getElementsByTagName('h3')[0].innerHTML;
			document.getElementsByTagName('h3')[0].innerHTML = document.getElementsByTagName('h3')[1].innerHTML;
			document.getElementsByTagName('h3')[1].innerHTML = teamAName;
			teamTempscore=teamAscore;
			teamAscore=teamBscore;
			teamBscore=teamTempscore;
			updateScoreboardNow();
		}
		
		function setTeams() {
			swal.setDefaults({
			  input: 'text',
			  confirmButtonText: 'Next &rarr;',
			  showCancelButton: true,
			  animation: false,
			  progressSteps: ['A', 'B']
			})
			var steps = [
			  {
				title: 'Team A',
				text: 'Mannschaftsname eingegeben'
			  },
			  {
				title: 'Team B',
				text: 'Mannschaftsname eingegeben'
				}
			]
			swal.queue(steps).then(function (result) {
					swal.resetDefaults();
					teamAName= result[0];
					teamBName = result[1];
					document.getElementsByTagName('h3')[0].innerHTML = teamAName;
					document.getElementsByTagName('h3')[1].innerHTML = teamBName;
			}, function () {
			  swal.resetDefaults();
			})
		}

		// Simple function to submit the current score values to the PHP script that updates the Arduino
		function submitForm(){
			var formData2 = padToSend(teamAscore,2) + "" + counteractual + "" + padToSend(teamBscore,2) + "" + padToSend(qtr,1);
			//swal(formData2)
            $.ajax({
                url: "scoreboard.php",
				method: "GET",
				data: {daten: formData2},
                cache: false,
                success: function () {},
                error: function () {}
            });
			}
		
		   window.onload = function () {
				initializeClock();
				updateScoreboardNow();
    }