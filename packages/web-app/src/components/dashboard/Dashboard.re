open Glamor;

open ReasonReactRouterDom;

module Styles = {
  let view = css([Selector("> * + *", [margin("16px 0 0")])]);
  let section = css([display("flex"), justifyContent("space-between")]);
  let sectionBlock = css([flex("1")]);
  let textPrimary = css([fontSize("1.25rem"), fontWeight("bold")]);
  let textDetail = css([fontSize("0.9rem"), color("#aaa")]);
};

type tournamentShape = {
  .
  "id": string,
  "discipline": string,
  "name": string,
  "status": string,
  "size": int,
  "teamSize": int
};

module ActiveTournamentsQuery = {
  [@bs.module "graphql-tag"] external gql : ReasonApolloTypes.gql = "default";
  let query =
    [@bs]
    gql(
      {|
  query ActiveTournaments($key: String!, $page: Int!, $perPage: Int!) {
    organization(key: $key) {
      key
      tournaments(
        status: [NEW, IN_PROGRESS]
        sort: { key: "createdAt", order: DESC }
        page: $page
        perPage: $perPage
      ) {
        id
        discipline
        name
        status
        size
        teamSize
      }
    }
  }
|}
    );
  type tournament = tournamentShape;
  type organization = {
    .
    "key": string,
    "tournaments": array(tournament)
  };
  type data = {. "organization": organization};
  type response = data;
  type variables = {
    .
    "key": string,
    "page": int,
    "perPage": int
  };
};

module ActiveTournaments =
  ConfigureApollo.Client.Query(ActiveTournamentsQuery);

module FinishedTournamentsQuery = {
  [@bs.module "graphql-tag"] external gql : ReasonApolloTypes.gql = "default";
  let query =
    [@bs]
    gql(
      {|
  query FinishedTournaments($key: String!, $page: Int!, $perPage: Int!) {
    organization(key: $key) {
      key
      tournaments(
        status: [FINISHED]
        sort: { key: "createdAt", order: DESC }
        page: $page
        perPage: $perPage
      ) {
        id
        discipline
        name
        status
        size
        teamSize
      }
    }
  }
|}
    );
  type tournament = tournamentShape;
  type organization = {
    .
    "key": string,
    "tournaments": array(tournament)
  };
  type data = {. "organization": organization};
  type response = data;
  type variables = {
    .
    "key": string,
    "page": int,
    "perPage": int
  };
};

module FinishedTournaments =
  ConfigureApollo.Client.Query(FinishedTournamentsQuery);

let component = ReasonReact.statelessComponent("Dashboard");

let make = (~match: match, _children) => {
  let organizationKey = Js.Dict.get(match##params, "organizationKey");
  let renderTournamentsList =
      (~tournaments: array(tournamentShape), ~organizationKey, ~labelEmptyList) =>
    if (Array.length(tournaments) == 0) {
      <div> (ReasonReact.stringToElement(labelEmptyList)) </div>;
    } else {
      <ul>
        (
          ReasonReact.arrayToElement(
            Array.map(
              tournament => {
                let tournamentId = tournament##id;
                <Link
                  key=tournament##id
                  to_={j|/$organizationKey/tournament/$tournamentId|j}>
                  <li>
                    <div className=Styles.textPrimary>
                      (ReasonReact.stringToElement(tournament##name))
                    </div>
                    <div className=Styles.textDetail>
                      (ReasonReact.stringToElement(tournament##discipline))
                    </div>
                  </li>
                </Link>;
              },
              tournaments
            )
          )
        )
      </ul>;
    };
  {
    ...component,
    render: _self =>
      switch organizationKey {
      | None =>
        Js.log("Error: organizationKey param is not defined!");
        /* Throw an error? */
        ReasonReact.nullElement;
      | Some(orgKey) =>
        <div className=Styles.view>
          <div className=Styles.section>
            <FetchUser>
              (
                response =>
                  switch response {
                  | Loading => <Welcome />
                  | Failed(error) =>
                    Js.log(error);
                    ReasonReact.nullElement;
                  | Loaded(result) => <Welcome name=result##me##name />
                  }
              )
            </FetchUser>
          </div>
          <div className=Styles.section>
            <div className=Styles.sectionBlock>
              <h3> (ReasonReact.stringToElement("Active tournaments")) </h3>
              <ActiveTournaments
                variables={"key": orgKey, "page": 1, "perPage": 20}>
                (
                  response =>
                    switch response {
                    | Loading => <LoadingSpinner />
                    | Failed(error) =>
                      Js.log(error);
                      ReasonReact.nullElement;
                    | Loaded(result) =>
                      let tournaments = result##organization##tournaments;
                      renderTournamentsList(
                        ~labelEmptyList=
                          "There are no active tournaments at the moment",
                        ~tournaments,
                        ~organizationKey=orgKey
                      );
                    }
                )
              </ActiveTournaments>
            </div>
            <div className=Styles.sectionBlock>
              <h3> (ReasonReact.stringToElement("Finished tournaments")) </h3>
              <FinishedTournaments
                variables={"key": orgKey, "page": 1, "perPage": 20}>
                (
                  response =>
                    switch response {
                    | Loading => <LoadingSpinner />
                    | Failed(error) =>
                      Js.log(error);
                      ReasonReact.nullElement;
                    | Loaded(result) =>
                      let tournaments = result##organization##tournaments;
                      renderTournamentsList(
                        ~labelEmptyList=
                          "There are no active tournaments at the moment",
                        ~tournaments,
                        ~organizationKey=orgKey
                      );
                    }
                )
              </FinishedTournaments>
            </div>
          </div>
          <div className=Styles.section>
            <Link to_="/new">
              (ReasonReact.stringToElement("Create new tournament"))
            </Link>
          </div>
        </div>
      }
  };
};

let default =
  ReasonReact.wrapReasonForJs(~component, jsProps =>
    make(~match=jsProps##_match, [||])
  );