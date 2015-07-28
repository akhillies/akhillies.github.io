$(document).ready( function() {
        $('.imagery a[data-toggle="tab"]').on("hide.bs.tab", function(event) {
            console.log('#' + event.target);
            if(history.pushState)
            {
                history.pushState(null, null, location.hash + '/' + event.target);
            }
            else
            {
                location.hash += '/' + event.target;
            }
        });
    });
