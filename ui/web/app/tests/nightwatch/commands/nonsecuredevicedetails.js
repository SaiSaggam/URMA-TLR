exports.command = function(name, fw, boot) {
  
  this   
      
      .waitForElementVisible(".home-panel.panel-full-width",5000)
      .assert.elementPresent(".panel-wide>img")
      .assert.containsText(".home-page-content-section>ul>li:nth-of-type(1)", name)
      .assert.containsText(".home-page-content-section>ul>li:nth-of-type(2)", fw)
      .assert.containsText(".home-page-content-section>ul>li:nth-of-type(3)", boot)

      .assert.containsText(".panel-wide--half:nth-of-type(1)>li:nth-of-type(1)","Power")
      .assert.attributeContains(".panel-wide--half:nth-of-type(1)>li:nth-of-type(1)>i","class",'circle-o status-green')
      .assert.containsText(".panel-wide--half:nth-of-type(1)>li:nth-of-type(2)","Service")
      .assert.attributeContains(".panel-wide--half:nth-of-type(1)>li:nth-of-type(2)>i",'class','circle-o')
      .assert.containsText(".panel-wide--half:nth-of-type(1)>li:nth-of-type(3)","WWAN")
      .assert.attributeContains(".panel-wide--half:nth-of-type(1)>li:nth-of-type(3)>i",'class','circle-o status-alert')
      .assert.containsText(".panel-wide--half:nth-of-type(2)>li:nth-of-type(1)","Signal 1")
      .assert.attributeContains(".panel-wide--half:nth-of-type(2)>li:nth-of-type(1)>i",'class','circle-o')
      .assert.containsText(".panel-wide--half:nth-of-type(2)>li:nth-of-type(2)","Signal 2")
      .assert.attributeContains(".panel-wide--half:nth-of-type(2)>li:nth-of-type(2)>i",'class','circle-o status-green')
      .assert.containsText(".panel-wide--half:nth-of-type(2)>li:nth-of-type(3)","Signal 3")
      .assert.attributeContains(".panel-wide--half:nth-of-type(2)>li:nth-of-type(3)>i",'class','circle-o')
      
      
  return this;
      
  };
