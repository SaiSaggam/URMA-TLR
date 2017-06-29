module.exports = function (client) {

  this.validate = function() {

    client

	.waitForElementVisible(".header-wrapper>header>h2>div", 5000)
	.assert.containsText(".header-wrapper>header>h2>div","LR54")
	.assert.elementPresent(".wizard-images>img")
	.assert.elementPresent(".wizard-content-text")
	.assert.containsText(".simStatus>p:nth-child(1)","Status")
	.assert.containsText(".simStatus>p:nth-child(2)","ICCID")
	.assert.containsText(".simStatus>p:nth-child(3)","Strength")
	.assert.containsText(".simStatus>p:nth-child(4)","Quality")
	.assert.containsText(".formRow:nth-child(1)","Country")
	.assert.containsText(".formRow:nth-child(2)","Provider")
	.assert.containsText(".formRow:nth-child(3)","APN")
	.assert.elementPresent(".formRow:nth-child(4)>div")
	.assert.elementPresent(".formRow:nth-child(5)>div")
	.assert.containsText(".formRow:nth-child(6)","SIM PIN")
	
	return client;
  };

  this.select = function(type) {

    client

	.waitForElementVisible(".wizard-images>img",5000)
	
				
	return client;
  };
    
};

