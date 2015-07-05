Feature: Contact us form
  In order for a customer to be able to contact us
  As a website user
  I need to be able to successfully submit the form

  Scenario: Login as admin.
    Given I am on "/"
    And I fill in the following:
        | Username | admin |
        | Password | admin |
    And press "Log in"
    Then I should see "jlkjlks"



