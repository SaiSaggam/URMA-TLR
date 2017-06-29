exports.command = function(ip, mask) {
  
  this   
      .useXpath()
      .waitForElementVisible(".//*[@id='react-app']/div/div/div[3]/div/div/h1", 5000)
      .click(".//*[@id='react-app']/div/div/div[3]/div/div/table/tbody/tr[1]/td[5]/a")
      .click(".//*[@id='react-app']/div/div/div[3]/div/div/div/form/div[3]/label/input")
      .clearValue(".//*[@id='react-app']/div/div/div[3]/div/div/div/form/div[4]/input")
      .setValue(".//*[@id='react-app']/div/div/div[3]/div/div/div/form/div[4]/input",ip)
      .clearValue(".//*[@id='react-app']/div/div/div[3]/div/div/div/form/div[5]/input")
      .setValue(".//*[@id='react-app']/div/div/div[3]/div/div/div/form/div[5]/input",mask)
      .click(".//*[@id='react-app']/div/div/div[2]/ul/li[2]/ul/li/ul/li/a")
      .assert.containsText(".//*[@id='react-app']/div/div/div[3]/div/div/table/tbody/tr[1]/td[4]",ip)
      
      
      .click(".//*[@id='react-app']/div/div/div[2]/ul/li[1]/a")
      .verify.containsText(".//*[@id='react-app']/div/div/div[3]/div/div/div[2]/div[1]/div[1]/div[2]/div/div/button","ETHERNET 1")
      //.verify.containsText(".//*[@id='react-app']/div/div/div[3]/div/div/div[2]/div[1]/div[1]/div[2]/div/ul/li[1]",ip)
      //.verify.containsText(".//*[@id='react-app']/div/div/div[3]/div/div/div[2]/div[1]/div[1]/div[2]/div/ul/li[4]",mask)
      //.verify.containsText(".//*[@id='react-app']/div/div/div[3]/div/div/div[2]/div[1]/div[1]/div[2]/div/ul/li[5]",gate)  
      
      
             
  return this;
      
  };
