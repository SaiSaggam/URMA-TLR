import React from 'react';
import MenuItem from './menu-item.jsx';
import RouterHelper from '../router-helper.js'


export default class Navigation extends React.Component {
    constructor(props){
        super(props);
    }

    handleOpenState(){
        this.props.setOpenState('navigation');
    }

    buildMenu(){
        let routerhelper = new RouterHelper();
        routerhelper.setSecurityEnabled(true);
        routerhelper.configureRoutes(this.props.routes, this.context.basePath);
        let mainMenuItems = routerhelper.createMenu();

        return mainMenuItems;
    }

    renderMenuItems(){
        return this.buildMenu().map(menuItem =>
            <MenuItem
                key={menuItem.name}
                level={1}
                setOpenState={this.props.setOpenState.bind(this)}
                openStateClass={this.props.openStateClass.bind(this)}
                openState={this.props.openState}
                menuItem={menuItem}/>
        );
    }

    render () {
        let navToggle = this.props.openState ? 'left' : 'right';
        return (
            <div className={'config-nav-main ' + this.props.openStateClass()}>
                <div className="main-menu-toggle">
                    <a className="main-menu-toggle-link" onClick={this.handleOpenState.bind(this)}>
                        <i className={'fa fa-angle-double-'+ navToggle}></i>
                    </a>
                </div>
                <ul className={'config-nav-main-list config-nav-main-list-first'}>
                    {this.renderMenuItems()}
                </ul>
            </div>
        )
    }
}

Navigation.propTypes = {
    setOpenState: React.PropTypes.func.isRequired,
    openStateClass: React.PropTypes.func.isRequired,
    openState: React.PropTypes.bool.isRequired
};

Navigation.contextTypes = {
    basePath: React.PropTypes.string.isRequired
};

