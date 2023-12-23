		// Show digits that are turned off on the scoreboard as greyed out 8s on the web interface
		function pad(num, size) {
			num=num.toString();
			if(num==0) {
				if(size==1) {
					return "0";
				}
				if(size==2) {
					return "<font color='dddddd'>8</font>0";
				}
				if(size==3) {
					return "<font color='dddddd'>88</font>0";
				}
			} else {
				if(num.length==size) {
					return num;
				}
				if(num.length==2 && size==3) {
					return "<font color='dddddd'>8</font>" + num;
				}
				if(num.length==1 && size==3) {
					return "<font color='dddddd'>88</font>" + num;
				}
				if(num.length==1 && size==2) {
					return "<font color='dddddd'>8</font>" + num;
				}
				if(num.length==1 && size==1) {
					return num;
				}
			}		
		}
		
		// Ensures we are sending the right sized string to the scoreboard to display - The scoreboard currently expects the string to be correct when it arrives at the Arduino so, this makes sure it is
		function padToSend(num, size) {
			num=num.toString();
			if(num==0) {
				if(size==1) {
					return "0";
				}
				if(size==2) {
					return "00";
				}
				if(size==3) {
					return "000";
				}
			} else {
				if(num.length==size) {
					return num;
				}
				if(num.length==2 && size==3) {
					return "0" + num;
				}
				if(num.length==1 && size==3) {
					return "00" + num;
				}
				if(num.length==1 && size==2) {
					return "0" + num;
				}
				if(num.length==1 && size==1) {
					return num;
				}
			}		
		}