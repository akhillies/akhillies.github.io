var app = angular.module('akbatra',
        [
            'ngRoute',
            'ngAnimate'
        ]);


app.config(['$routeProvider', '$locationProvider', function($routeProvider, $locationProvider) {
        $routeProvider
            .when("/", {templateUrl: "html/home.html", controller: "homeCtrl"})
            .when("/about", {templateUrl: "html/about.html", controller: "aboutCtrl"})
            .when("/coding", {templateUrl: "html/coding.html", controller: "codingCtrl"})
            .when("/imagery", {redirectTo: '/imagery/home'})
            .when("/imagery/:tabid", {templateUrl: "html/imagery.html", controller: "imageryCtrl"})
            .when("/404", {templateUrl: "html/error.html", controller: "errorCtrl"})
            .otherwise({redirectTo: '/404'});

        // $locationProvider.html5Mode({
 //            enabled: true
        // });
    }]);


app.controller('homeCtrl', function ($scope /*, $location, $http */) {
        $scope.pageClass = "homePage";
    });

app.controller('navbarCtrl', function ($scope, $location) { 
        $scope.isActive = function (viewLocation) { 
            return ($location.path() === viewLocation) || (viewLocation !== '/' && $location.path().startsWith(viewLocation));
        };
    });

app.controller('aboutCtrl', function ($scope) {
        $scope.pageClass = "aboutPage";
    });

app.controller('codingCtrl', function ($scope) {
        $scope.pageClass = "codingPage";
    });

app.controller('imageryCtrl', function ($scope, $routeParams) {
        $scope.pageClass = "imageryPage";
        $scope.isActive = function (tab) {
            return tab === $routeParams.tabid;
        };
    });

app.controller('errorCtrl', function ($scope) {
        $scope.pageClass = "errorPage";
    });

