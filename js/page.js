var app = angular.module('akbatra', [
    'ngRoute',
    'ngAnimate'
]);


app.config(['$routeProvider', function ($routeProvider) {
    $routeProvider
        .when("/", {templateUrl: "html/home.html", controller: "homeCtrl"})
        .when("/about", {templateUrl: "html/about.html", controller: "aboutCtrl"})
        .when("/coding", {templateUrl: "html/coding.html", controller: "codingCtrl"})
        .when("/imagery", {templateUrl: "html/imagery.html", controller: "imageryCtrl"})
        .when("/404", {templateUrl: "html/error.html", controller: "errorCtrl"})
        .otherwise({redirectTo: '/404'});
}]);

app.controller('homeCtrl', function ($scope /*, $location, $http */) {
    $scope.pageClass = "homePage";
});
app.controller('aboutCtrl', function ($scope) {
    $scope.pageClass = "aboutPage";
});
app.controller('codingCtrl', function ($scope) {
    $scope.pageClass = "codingPage";
});
app.controller('imageryCtrl', function ($scope) {
    $scope.pageClass = "imageryPage";
});
app.controller('errorCtrl', function ($scope) {
    $scope.pageClass = "errorPage";
});

app.controller('navbarCtrl', function ($scope, $location) { 
    $scope.isActive = function (viewLocation) { 
        return viewLocation === $location.path();
    };
});
