import React from 'react';
import ContextualLink from './contextual-link.jsx';

export default class MenuItem extends React.Component {
    constructor(props) {
        super(props);
        this.state = { open: false };
        this.toggleValue = 'right';
    }

    findMenuLevel() {
        switch(this.props.level){
            case 1:
                return 'first';
            case 2:
                return 'second';
            default:
                return 'third';
        }
    }

    //set open state of list item and change the class of the arrow icon when link is clicked.
    handleClick(e){
        if(!this.state.open && this.props.openState && this.props.menuItem.children.length > 0) {
            this.setState({open: true});
            this.toggleValue = 'down';
        } else if(!this.props.openState) {
            if(this.props.level === 1) {
                this.props.setOpenState('menu-item');
            }
            if(this.props.menuItem.children.length > 0){
                this.setState({open: true});
                this.toggleValue = 'down';
            }
        } else {
            this.setState({open: false});
            this.toggleValue = 'right';
        }
        // If no url was provided on this menu item don't follow link. (Router helper sets link to #)
        if (this.props.menuItem.url === '#') {
            e.preventDefault();
        }
    }

    //only give list element class of open if it is expanded AND the main menu is open.
    areAllOpen() {
        if (this.state.open && this.props.openState){
            return 'open';
        } else {
            return 'collapsed';
        }
    }

    getLocation(){
        let location = window.location.pathname;

        if(location === this.props.menuItem.url) {
            return 'active';
        }
    }

    render() {
        return (
            <li className={this.areAllOpen() +' config-nav-main-listitem config-nav-main-listitem-'+ this.findMenuLevel()}>
                <ContextualLink to={this.props.menuItem.url} className={this.getLocation()} onClick={this.handleClick.bind(this)}>
                    <div className='main-menu-image'>
                        {this.props.menuItem.nav.image ?
                            <i className={this.props.menuItem.nav.image}></i>: null}
                    </div>
                    <div className={'main-menu-label-wrapper ' + this.props.openStateClass()}>
                        {this.props.level >1?
                            <span className='expando'>
                                {this.props.menuItem.children.length > 0?
                                <i className={'fa fa-caret-'+ this.toggleValue}></i>: null}
                            </span> : null}
                        <span className={'main-menu-label menu-label-'+ this.findMenuLevel()}>
                            {this.props.menuItem.nav.menuLabel}
                        </span>
                    </div>
                </ContextualLink>
                { this.props.menuItem.children.length > 0 ?
                    <ul className='config-nav-main-list-child'>{this.props.menuItem.children.map(childItem =>
                        <MenuItem
                            key={childItem.name}
                            level={this.props.level + 1}
                            setOpenState={this.props.setOpenState.bind(this)}
                            openStateClass={this.props.openStateClass.bind(this)}
                            openState={this.props.openState}
                            menuItem={childItem}/>
                    )}
                    </ul>
                    : null}
            </li>
        )
    }
}

MenuItem.propTypes = {
    setOpenState: React.PropTypes.func.isRequired,
    openStateClass: React.PropTypes.func.isRequired,
    openState: React.PropTypes.bool.isRequired,
    menuItem: React.PropTypes.object.isRequired
};
