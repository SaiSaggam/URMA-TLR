class SimulateReboot {
    constructor(){
        this.disconnectTimeout = null;
        this.reconnectTimeout = null;
    }

    simulateReboot() {
        this.disconnectTimeout = setTimeout(()=>{
            Meteor.disconnect();
        }, 2000);

        this.reconnectTimeout = setTimeout(()=>{
            Meteor.reconnect();
        }, 20000);
    };

    cleanUp() {
        clearTimeout(this.disconnectTimeout);
        clearTimeout(this.reconnectTimeout);
    }
}

export default Simulators = {
    SimulateReboot: new SimulateReboot()
};