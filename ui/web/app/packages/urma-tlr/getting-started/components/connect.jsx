import {default as React, Component, PropTypes } from 'react';


export default class Connect extends Component {
    constructor(props) {
        super(props);
        this.handleClick = this.handleClick.bind(this);
    }

    componentWillMount(){
        this.props.buttonConfig('/getting-started/welcome', '/getting-started/cellular/0');
        this.props.enableNext();
    }
    
    handleClick(event) {
        if(event.target.value === "Ethernet"){
            this.props.buttonConfig('/getting-started/welcome', '/getting-started/ethernet');
        }else{
            this.props.buttonConfig('/getting-started/welcome', '/getting-started/cellular/0');
        }
        this.props.enableNext();
    }

    render(){
        return (
             <div className="welcome-getting-started">
                <div className="wizard-content-area-left">
                   <div className="wizard-images">
                        <img src="/images/LR_frontPanel.jpg" alt="Front Panel"/>
                        <p className="content-margin-left bold"><b>Front Panel</b></p>
                        <img src="/images/LR_backPanel.jpg" alt="Back Panel"/>
                        <p className="content-margin-left bold" ><b>Back Panel</b></p>
                    </div>
                    <div className="wizard-content-text">
                        <h1>Choose to connect with Ethernet or cellular.</h1>
                         <p>Select the desired WAN connection type from the choices to the right.</p>
                         <p>Ensure that the appropriate hardware is connected, such as external antennae, Ethernet cables or SIM cards.</p>
                         <p>Refer to the quick start guide as needed to help with hardware connections.</p>
                    </div> 
                </div>
                <div className="wizard-content-area-right">
                    <form>
                        <p>Select WAN connection type</p>
                        <div className="box">
                            <label><input type="radio" className="form-control" name="connectionType" value="Cellular" defaultChecked onChange={this.handleClick}/>Cellular</label>
                        </div>
                        <div className="box">
                            <label><input type="radio" className="form-control" name="connectionType" value="Ethernet" onChange={this.handleClick}/>Ethernet</label>
                        </div>
                        
                        <p>Click NEXT to continue</p>
                    </form>  
                </div>
            </div>
        )
    }
};