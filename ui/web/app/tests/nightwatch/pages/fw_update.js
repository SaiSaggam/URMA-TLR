module.exports = function (client) {
  var current_fw;
  var avail_fw;

  this.validate = function() {

    client

	.waitForElementVisible(".config-main>div>h1",5000)
      	.assert.containsText(".config-main>div>h1","Firmware Update")
	.assert.containsText(".box.firmware-update-current-version", "Current Version")
	.getText('.firmware-update-message>p' ,function(result){
		var fw_status = result.value;
		if (fw_status === "The device's firmware is up to date") {
		  client
			.assert.elementNotPresent("firmware-update-option-auto")
			.assert.elementPresent("firmware-update-option-manual")
		} else {
            	  client
			.assert.elementPresent("firmware-update-option-auto")
			.assert.cssClassNotPresent(".next","disabled")
			.assert.elementPresent("firmware-update-option-manual")
		}
        })
	.click(".firmware-update-option-manual")
	.assert.containsText(".firmware-update-manual.margin-top","firmware file from your computer")
	.assert.elementPresent("#fwFileUploadForm>input")
	.assert.attributeEquals(".next","disabled","")

    return client;
  };

  this.manual = function(path) {

     client

	.waitForElementVisible(".config-main>div>h1",5000)
      	.assert.containsText(".config-main>div>h1","Firmware Update")
	.assert.containsText(".box.firmware-update-current-version", "Current Version")
	.click(".firmware-update-option-manual")
      	.waitForElementVisible("#fwFileUploadForm>input",5000)
	.setValue('#fwFileUploadForm>input', require('path').resolve(__dirname + '/',client.globals.device1.path))
	.click(".next")
	.waitForElementVisible(".progress-bar-wrapper",5000)
      	.WaitForText(".dialog--jss-0-0>div>div>div:nth-child(1)", function(text) {return text === "Applying new firmware, please wait.";})
	.waitForElementVisible(".dialog--jss-0-0",10000)
      	.waitForElementVisible(".reboot-msg>div>h5",50000)
      	.assert.elementPresent(".countdown-timer")
	.waitForElementVisible(".welcome-getting-started>h1",150000)
			
    return client;
  };

  this.auto = function() {

     client

	.waitForElementVisible(".config-main>div>h1",5000)
      	.assert.containsText(".config-main>div>h1","Firmware Update")
	.assert.containsText(".box.firmware-update-current-version", "Current Version")
	.getText(".box.firmware-update-current-version>span>span:nth-child(2)",function(result) {
             current_fw = result.value;
             this
               .getText(".firmware-update-option-auto",function(result) {
                	avail_fw = result.value;
                        console.log('Current Firmware version is ',current_fw,' and ', 'Latest firmware version is ', avail_fw);
               		this
                          if (current_fw === avail_fw) { 
                          	client.assert.attributeEquals(".next","disabled","")
				console.log('Firmware is alerady the latest');
                          }
                          else {
                          	client.click(".firmware-update-option-auto")
			   	console.log('We have a new firmware version available');
                          }
			console.log(avail_fw);
			client
				.click(".next")
				.waitForElementVisible(".progress-bar-wrapper",5000)
      				.WaitForText(".dialog--jss-0-0>div>div>div:nth-child(1)", function(text) {return text === "Applying new firmware, please wait.";})
      				.waitForElementVisible(".dialog--jss-0-0",10000)
				.waitForElementVisible(".reboot-msg>div>h5",50000)
      				.assert.elementPresent(".countdown-timer")
	      			.waitForElementVisible(".welcome-getting-started>h1",150000)
      			return this;
			
			})
			
      		})
			
    return client;
  };
	
};

