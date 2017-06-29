module.exports = {
  tags:['setupwizardmanual'],
  "Set up Wizard flow with manual firmware update" : function (client) {
    client

      .url(client.globals.device1.url)
      .waitForElementVisible(".header-wrapper>header>h2>img",5000)
      
      //Welcome page
      .waitForElementVisible(".welcome-getting-started>h1",5000)
      .assert.containsText(".header-wrapper>header>h2>div","LR54")
      .assert.containsText(".welcome-getting-started>h1","Welcome")
      .assert.elementPresent(".wizard-content-area-left")
      .assert.elementPresent(".wizard-content-area-right>img")
      .assert.containsText(".wizard-links>a:nth-child(1)","Quick Start Guide")
      
      .assert.containsText(".wizard-links>a:nth-child(2)","User Manual")

      .click(".next")
           
      //Login 
      .waitForElementVisible(".config-main>div>div>h1",5000)
      .assert.containsText(".config-main>div>div>h1","Login")
      .assert.elementPresent(".wizard-images>img[alt='Front Panel']")
      .assert.elementPresent(".wizard-images>img[alt='Back Panel']")
      .assert.elementPresent(".wizard-content-text")
      .waitForElementVisible(".form-control[type=text]",5000)
      .clearValue(".form-control[type=text]")
      .setValue(".form-control[type=text]",client.globals.device1.username)
      .waitForElementVisible(".form-control[type=password]",5000)
      .clearValue(".form-control[type=password]")
      .setValue(".form-control[type=password]",client.globals.device1.password)
      .click(".next")
      
      //Firmware update
      .waitForElementVisible(".config-main>div>h1",5000)
      .assert.containsText(".config-main>div>h1","Firmware Update")
      .click(".firmware-update-option-manual")
      .waitForElementVisible("#fwFileUploadForm>input",5000)
      
      .setValue('#fwFileUploadForm>input', require('path').resolve(__dirname + '/',client.globals.device1.path))

      .click(".next")

      .waitForElementVisible(".progress-bar-wrapper",5000)
      
      .WaitForText(".dialog--jss-0-0>div>div>div:nth-child(1)", function(text) {return text === "Applying new firmware, please wait.";})

      .waitForElementVisible(".dialog--jss-0-0",10000)
      
      .waitForElementVisible(".reboot-msg>div>h5",50000)
      .assert.elementPresent(".countdown-timer")
      
      .waitForElementVisible(".welcome-getting-started>h1",250000)
      .click(".next")
      
      .waitForElementVisible(".form-control[type=text]",5000)
      .clearValue(".form-control[type=text]")
      .setValue(".form-control[type=text]",client.globals.device1.username)
      .waitForElementVisible(".form-control[type=password]",5000)
      .clearValue(".form-control[type=password]")
      .setValue(".form-control[type=password]",client.globals.device1.password)
      .click(".next")

      .waitForElementVisible(".config-main>div>h1",5000)
      .assert.elementPresent(".box.firmware-update-current-version>span>span:nth-child(2)")
      //.verify.containsText(".box.firmware-update-current-version>span>span",client.globals.defaultpath.path)
          
      
      .end();
  }
};
      
