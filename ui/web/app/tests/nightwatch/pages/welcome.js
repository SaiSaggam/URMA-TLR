module.exports = function (client) {

  this.next = function() {

    client

	.waitForElementVisible(".header-wrapper>header>h2>div", 5000)
	.click(".next")

    return client;
  };

  this.links = function() {

     client

	//testing the Quick start guide link
	.assert.containsText(".wizard-links>a:nth-child(1)","Quick Start Guide")
	.click(".wizard-links>a:nth-child(1)")
	.windowHandles(function(result) {
      	client.assert.equal(result.value.length, 2, 'Quick start guide');
        var guideWindowHandle = result.value[1];
          client.switchWindow(guideWindowHandle)
		.assert.urlContains('digidocs')
		.assert.elementPresent(".logo>a>img")
        	.closeWindow(guideWindowHandle)
	})
	.windowHandles(function(result) {
        client.assert.equal(result.value.length, 1)
        client.switchWindow(result.value[0])
	})

	//testing the User manual link
	.assert.containsText(".wizard-links>a:nth-child(2)","User Manual")
	.click(".wizard-links>a:nth-child(2)")
	.windowHandles(function(result) {
      		client.assert.equal(result.value.length, 2, 'User manual');
        	var umWindowHandle = result.value[1];
          	client.switchWindow(umWindowHandle)
			.assert.urlContains('digidocs')
  		
        		.closeWindow(umWindowHandle)
		})
        .windowHandles(function(result) {
        	client.assert.equal(result.value.length, 1)
        	client.switchWindow(result.value[0])
		})
	
    return client;
  };

  this.validate = function() {

     client

	.waitForElementVisible(".header-wrapper>header>h2>div", 5000)
	.assert.containsText(".header-wrapper>header>h2>div","LR54")
	.assert.containsText(".welcome-getting-started>h1","Getting Started Wizard")
	.assert.elementPresent(".wizard-content-area-left")
	.assert.elementPresent(".wizard-content-area-right>img")
	.assert.containsText(".wizard-links>a:nth-child(1)","Quick Start Guide")
	.assert.containsText(".wizard-links>a:nth-child(2)","User Manual")
			
  return client;
  };	
	
};

