exports.command = function() {
  
  this   
      
      .waitForElementVisible(".config-main>div", 5000)
      .elements(".table.table-striped>tbody>tr>td:nth-of-type(3)", value,function(r){
                                client.perform(function(){
                                console.log("LIST", r);
                                })
      })
      
      .click(".table.table-striped>tbody>tr:nth-of-type(1)>td>a")

      
  return this;
      
  };
