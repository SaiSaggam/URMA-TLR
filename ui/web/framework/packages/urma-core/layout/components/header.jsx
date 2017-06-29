import LoginButtons from '../../security/components/login-buttons.jsx';
import React from 'react';

/**
 * Main header for the device UI. Includes the banner with logo and title as well as the login buttons.
 * @type {Header}
 */
export default class Header extends React.Component {
    render() {
        return (
            <div className='header-wrapper'>
                <header>
                    <h2>
                        {(this.props.logo) ?
                            <img src={this.props.logo} alt={''}/>
                            :
                            null
                        }
                        {this.props.title}
                    </h2>
                    {this.props.loginService ?
                        <LoginButtons loginService={this.props.loginService}/>
                        :
                        null
                    }
                </header>
            </div>
        )
    }
};

Header.propTypes = {
    title: React.PropTypes.string.isRequired,
    logo: React.PropTypes.string,
    loginService: React.PropTypes.object
};
