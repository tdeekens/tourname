module LoggedInUserQuery = {
  [@bs.module] external gql : ReasonApolloTypes.gql = "graphql-tag";
  let query =
    [@bs]
    gql(
      {|
      query LoggedInUser {
        me {
          id
          name
          email
          picture
          availableOrganizations {
            key
            name
          }
        }
      }
    |}
    );
  type availableOrganizations = {
    .
    "key": string,
    "name": string
  };
  type user = {
    .
    "id": string,
    "name": string,
    "email": string,
    "picture": string,
    "availableOrganizations": list(availableOrganizations)
  };
  type data = {. "me": user};
  type response = data;
  type variables;
};

module FetchLoggedInUser = Apollo.Client.Query(LoggedInUserQuery);

let component = FetchLoggedInUser.component;

let make = FetchLoggedInUser.make;