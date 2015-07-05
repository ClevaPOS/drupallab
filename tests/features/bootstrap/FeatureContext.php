<?php

use Behat\Behat\Context\ClosuredContextInterface,
    Behat\Behat\Context\TranslatedContextInterface,
    Behat\Behat\Context\BehatContext,
    Behat\Behat\Exception\PendingException;
use Behat\Gherkin\Node\PyStringNode,
    Behat\Gherkin\Node\TableNode;

use Behat\Mink\Mink,
    Behat\Mink\Session,
    Behat\Mink\Driver\Selenium2Driver,
    Behat\MinkExtension\Context\MinkContext,
    Behat\Behat\Context\Step\Given,
    Behat\Behat\Context\Step\When,
    Behat\Behat\Context\Step\Then;


require_once 'PHPUnit/Autoload.php';
require_once 'PHPUnit/Framework/Assert/Functions.php';



//
// Require 3rd-party libraries here:
//
//   require_once 'PHPUnit/Autoload.php';
//   require_once 'PHPUnit/Framework/Assert/Functions.php';
//

/**
 * Features context.
 */
class FeatureContext extends MinkContext
{
    /**
     * Initializes context.
     * Every scenario gets it's own context object.
     *
     * @param array $parameters context parameters (set them up through behat.yml)
     */
    public function __construct(array $parameters)
    {
        // Initialize your context here
    }


    /**
     * @Given /^I am login as user "([^"]*)"$/
     */
    public function iAmLoginAsUser($arg1)
    {

       
        // make sure we're on the drupal page
        $user = $this->_parameters['credentials']['drupal'][$arg1];
        $this->getSession()->visit($this->locatePath('/user'));
        /*
         * I don't know why we need this
         */
        $this->spin(function($context) {
          return ($context->getSession()->getPage()->findById("edit-name") );
        });

        return array(
          new Given('I fill in "name" with "' . $user['username'] . '"'),
          new Given('I fill in "pass" with "' . $user['password'] . '"'),
          new When('I press "Log in"'),
        );
    }

    /**
     * Take screenshot when step fails.
     * Works only with Selenium2Driver.
     *
     * @AfterStep
     */
    public function takeScreenshotAfterFailedStep($event)
    {

        if (4 === $event->getResult() && !empty($this->_parameters['screenshots_dir'])) {

          $driver = $this->getSession()->getDriver();
          if (!($driver instanceof Selenium2Driver)) {
            //throw new UnsupportedDriverActionException('Taking screenshots is not supported by %s, use Selenium2Driver instead.', $driver);
            return;
          }

          $screenshot = $driver->getWebDriverSession()->screenshot();
          file_put_contents($this->_parameters['screenshots_dir'] . '/' . hash('md5', time()) . '.png', base64_decode($screenshot));
        }
    }

}
