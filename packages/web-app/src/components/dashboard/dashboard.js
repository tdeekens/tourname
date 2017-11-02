import React from 'react';
import PropTypes from 'prop-types';
import { withRouter } from 'react-router';
import { Link } from 'react-router-dom';
import gql from 'graphql-tag';
import { graphql } from 'react-apollo';
import styled from 'styled-components';
import { compose } from 'recompose';
import withUser from '../with-user';
import Loading from '../loading';
import Welcome from '../welcome';

const View = styled.div`
  > * + * {
    margin: 16px 0 0 0;
  }
`;
const Section = styled.div`
  display: flex;
  justify-content: space-between;
`;
const SectionBlock = styled.div`
  flex: 1;
`;
const SectionBlockTitle = styled.h3``;
const Button = styled.button``;
const List = styled.ul``;
const ListItem = styled.li``;
const TextPrimary = styled.div`
  font-size: 1.25rem;
  font-weigth: bold;
`;
const TextDetail = styled.div`
  font-size: 0.9rem;
  color: #aaa;
`;

const ActiveTournaments = gql`
  query ActiveTournaments($key: String!, $page: Int!, $perPage: Int!) {
    organizationByKey(key: $key) {
      id
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
`;

const FinishedTournaments = gql`
  query FinishedTournaments($key: String!, $page: Int!, $perPage: Int!) {
    organizationByKey(key: $key) {
      id
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
`;

class Dashboard extends React.PureComponent {
  static propTypes = {
    match: PropTypes.shape({
      params: PropTypes.shape({
        organizationKey: PropTypes.string.isRequired,
      }).isRequired,
    }).isRequired,
    fullName: PropTypes.string.isRequired,
    activeTournaments: PropTypes.shape({
      loading: PropTypes.bool.isRequired,
      organizationByKey: PropTypes.shape({
        tournaments: PropTypes.arrayOf(
          PropTypes.shape({
            id: PropTypes.string.isRequired,
            discipline: PropTypes.string.isRequired,
            name: PropTypes.string.isRequired,
            status: PropTypes.string.isRequired,
            size: PropTypes.string.isRequired,
            teamSize: PropTypes.number.isRequired,
          })
        ).isRequired,
      }),
    }),
    finishedTournaments: PropTypes.shape({
      loading: PropTypes.bool.isRequired,
      organizationByKey: PropTypes.shape({
        tournaments: PropTypes.arrayOf(
          PropTypes.shape({
            id: PropTypes.string.isRequired,
            discipline: PropTypes.string.isRequired,
            name: PropTypes.string.isRequired,
            status: PropTypes.string.isRequired,
            size: PropTypes.string.isRequired,
            teamSize: PropTypes.number.isRequired,
          })
        ).isRequired,
      }),
    }),
  };

  renderActiveTournamentsList = () => {
    const { tournaments } = this.props.activeTournaments.organizationByKey;
    if (tournaments.length === 0)
      return <div>{'There are no active tournaments at the moment'}</div>;
    return (
      <List>
        {tournaments.map(tournament => (
          <Link
            key={tournament.id}
            to={`/${this.props.match.params
              .organizationKey}/tournaments/${tournament.id}`}
          >
            <ListItem>
              <TextPrimary>{tournament.name}</TextPrimary>
              <TextDetail>{tournament.discipline}</TextDetail>
            </ListItem>
          </Link>
        ))}
      </List>
    );
  };

  renderFinishedTournamentsList = () => {
    const { tournaments } = this.props.finishedTournaments.organizationByKey;
    if (tournaments.length === 0)
      return <div>{'There are no finished tournaments at the moment'}</div>;
    return (
      <List>
        {tournaments.map(tournament => (
          <Link
            key={tournament.id}
            to={`/${this.props.match.params
              .organizationKey}/tournaments/${tournament.id}`}
          >
            <ListItem>
              <TextPrimary>{tournament.name}</TextPrimary>
              <TextDetail>{tournament.discipline}</TextDetail>
            </ListItem>
          </Link>
        ))}
      </List>
    );
  };

  render() {
    return (
      <View>
        <Section>
          <Welcome name={this.props.fullName} />
          {/* TODO: show some statistics (and maybe a chart?), e.g.:
        - number of tournaments played
        - number of wins
        - number of active tournaments
      */}
        </Section>
        <Section>
          <SectionBlock>
            <SectionBlockTitle>{'Active tournaments'}</SectionBlockTitle>
            {this.props.activeTournaments.loading ? (
              <Loading />
            ) : (
              this.renderActiveTournamentsList()
            )}
          </SectionBlock>
          <SectionBlock>
            <SectionBlockTitle>{'Finished tournaments'}</SectionBlockTitle>
            {this.props.finishedTournaments.loading ? (
              <Loading />
            ) : (
              this.renderFinishedTournamentsList()
            )}
          </SectionBlock>
        </Section>
        <Section>
          <Link to={`/new`}>
            <Button>{'Create new tournament'}</Button>
          </Link>
        </Section>
      </View>
    );
  }
}

export default compose(
  withRouter,
  withUser(data => ({ fullName: data.me.name })),
  graphql(ActiveTournaments, {
    name: 'activeTournaments',
    options: ownProps => ({
      variables: {
        key: ownProps.match.params.organizationKey,
        page: 1,
        perPage: 20,
      },
    }),
  }),
  graphql(FinishedTournaments, {
    name: 'finishedTournaments',
    options: ownProps => ({
      variables: {
        key: ownProps.match.params.organizationKey,
        page: 1,
        perPage: 20,
      },
    }),
  })
)(Dashboard);
