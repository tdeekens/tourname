open ReasonReactRouterDom;

[@bs.module "../tournaments-list"]
external tournamentsList : ReasonReact.reactClass = "default";

[@bs.module "../tournament-detail"]
external tournamentDetail : ReasonReact.reactClass = "default";

module NotFound = {
  let component = ReasonReact.statelessComponent("NotFound");
  let make = _children => {
    ...component,
    render: _self => <div> ("404 Not Found" |> ReasonReact.string) </div>,
  };
};

[@bs.scope "localStorage"] [@bs.val]
external setItem : (string, string) => unit = "";

module RouterMatch =
  SpecifyRouterMatch({
    type params = {. "organizationKey": string};
  });

let component = ReasonReact.statelessComponent("ApplicationContent");

let make = (~match: RouterMatch.match, _children) => {
  let organizationKey = match##params##organizationKey;
  {
    ...component,
    render: _self => {
      let organizationQuery =
        FetchOrganization.OrganizationQuery.make(~key=organizationKey, ());
      <FetchOrganization variables=organizationQuery##variables>
        ...(
             ({result}) =>
               switch (result) {
               | Loading => "Loading..." |> ReasonReact.string
               | Error(_error) => <NotFound />
               | Data(_response) =>
                 <div>
                   <Route
                     path="/:organizationKey"
                     render=(
                       _renderFunc => {
                         setItem("organizationKey", organizationKey);
                         ReasonReact.null;
                       }
                     )
                   />
                   <Switch>
                     <Route
                       exact=true
                       path="/:organizationKey"
                       component=Dashboard.reactClass
                     />
                     <Route
                       exact=true
                       path="/:organizationKey/tournaments"
                       component=tournamentsList
                     />
                     <Route
                       exact=true
                       path="/:organizationKey/tournament/:tournamentId"
                       component=TournamentDetail.reactClass
                     />
                   </Switch>
                 </div>
               }
           )
      </FetchOrganization>;
    },
  };
};

let reactClass =
  ReasonReact.wrapReasonForJs(~component, jsProps =>
    make(~match=jsProps##_match, [||])
  );