import React from 'react';
import PropTypes from 'prop-types';
import gql from 'graphql-tag';
import { graphql } from 'react-apollo';
import { Breadcrumb, Breadcrumbs } from '../breadcrumbs';
import Loading from '../loading';
import TournamentDetailAddPlayersForm from '../tournament-detail-add-players-form';

const TournamentDetailQuery = gql`
  query TournamentDetail($id: String!) {
    tournament(id: $id) {
      id
      createdAt
      lastModifiedAt
      discipline
      name
      organizationKey
      status
      teamSize
      teams {
        key
        players {
          id
          email
          name
          picture
        }
      }
    }
  }
`;

class TournamentDetail extends React.PureComponent {
  static propTypes = {
    match: PropTypes.shape({
      params: PropTypes.shape({
        organizationKey: PropTypes.string.isRequired,
        tournamentId: PropTypes.string.isRequired,
      }).isRequired,
    }).isRequired,
    tournamentDetail: PropTypes.shape({
      loading: PropTypes.bool.isRequired,
      tournament: PropTypes.shape({
        id: PropTypes.string.isRequired,
        createdAt: PropTypes.string.isRequired,
        lastModifiedAt: PropTypes.string.isRequired,
        discipline: PropTypes.string.isRequired,
        name: PropTypes.string.isRequired,
        organizationKey: PropTypes.string.isRequired,
        status: PropTypes.string.isRequired,
        teamSize: PropTypes.number.isRequired,
        teams: PropTypes.array.isRequired,
      }),
    }),
  };
  render() {
    if (this.props.tournamentDetail.loading) return <Loading />;
    const { tournament } = this.props.tournamentDetail;
    return (
      <div>
        <Breadcrumbs separator="//">
          <Breadcrumb linkTo={`/${this.props.match.params.organizationKey}`}>
            {this.props.match.params.organizationKey}
          </Breadcrumb>
          <Breadcrumb>{'Tournament'}</Breadcrumb>
        </Breadcrumbs>
        <p>{`Name: ${tournament.name}`}</p>
        <p>{`Status: ${tournament.status}`}</p>
        <p>{`Discipline: ${tournament.discipline}`}</p>
        <p>{`Max players for each team: ${tournament.teamSize}`}</p>

        {tournament.status === 'NEW' ? (
          <TournamentDetailAddPlayersForm
            teams={tournament.teams}
            teamSize={tournament.teamSize}
          />
        ) : null}
      </div>
    );
  }
}

export default graphql(TournamentDetailQuery, {
  alias: 'withTournament',
  name: 'tournamentDetail',
  options: ownProps => ({
    variables: {
      id: ownProps.match.params.tournamentId,
    },
  }),
})(TournamentDetail);
