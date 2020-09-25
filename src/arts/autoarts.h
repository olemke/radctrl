#include <arts.h>
#include <auto_md.h>
#include <auto_workspace.h>
#include <global_data.h>
#include <m_append.h>
#include <m_basic_types.h>
#include <m_conversion.h>
#include <m_copy.h>
#include <m_delete.h>
#include <m_extract.h>
#include <m_general.h>
#include <m_gridded_fields.h>
#include <m_ignore.h>
#include <m_nc.h>
#include <m_reduce.h>
#include <m_select.h>
#include <m_xml.h>

namespace ARTS::Var {
/*! Azimuthal angle grid.

The azimutal angle grid, on which the intensity field is stored.
This grid is used for RT calculations inside the cloudbox,
therefore one has to define it if the cloudbox is activated by
the flag *cloudbox_on*.
The grid must be sorted in increasing order, with no repetitions.

Usage:      Set by the user.

Unit:       degrees
*/
Vector& aa_grid(Workspace& ws) noexcept { return *static_cast<Vector*>(ws[0]); }

/*! Azimuth angle index for scattering calculations.

This variable is used in methods used for computing scattering
properties.
It holds the information about the azimuth angles for which the
scattering calculations are done.  The angles used for computing
scattering properties of particles can be different from that used
for radiative transfer calculation.

Usage:    Method output.
*/
Index& aa_index(Workspace& ws) noexcept { return *static_cast<Index*>(ws[1]); }

/*! HITRAN Collision Induced Absorption (CIA) Data.

This variable holds HITRAN CIA data (binary absorption
cross-sections). The data itself is described in: Richard, C. et al.
(2012), New section of the HITRAN database: Collision-induced
absorption (CIA), J. Quant. Spectrosc. Radiat. Transfer, 113,
1276-1285, doi:10.1016/j.jqsrt.2011.11.004.

The binary absorption cross-sections have to be multiplied with the
densities of both molecules to get absorption coefficients.

Dimensions:

The outer array dimension in the ArrayOfArrayOfCIARecord is the same
as that of *abs_species*. There will be CIA data only for those
species that contain a CIA tag, for all other species it will be
empty. The inner array dimension corresponds to the number of CIA tags
for this species (there could be for example both N2-N2 and N2-H2) in
the same species.

The CIA *abs_species* tags are described in *abs_speciesSet*.

Each individual CIARecord holds the complete information from one
HITRAN CIA file. For the given pair of molecules A HITRAN CIA data
file can hold several datasets (data for different temperatures but
fixed frequency range).

Units:
Frequencies: Hz
Binary absorption cross-sections: m^5*molecule^-2
*/
ArrayOfCIARecord& abs_cia_data(Workspace& ws) noexcept {
  return *static_cast<ArrayOfCIARecord*>(ws[2]);
}

/*! The matrix of total scalar absorption coefficients.

Contains the gas absorption summed over all species as a function of
*f_grid* and *abs_p*, i.e., for a single atmospheric profile.

This variable is not used explicitly in a standard calculation, where
absorption comes from the lookup table *abs_lookup*. However, it is
useful for testing the methods that actually calculate line-by-line
absorption, which have this variable as output. These methods are
called internally by the method *abs_lookupCalc*, which generates
the lookup table.

Dimensions: [f_grid, abs_p]

Unit: 1/m
*/
Matrix& abs_coef(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[3]);
}

/*! Scalar absorption coefficients individually per tag group.

The Array contains one matrix of absorption coefficients for each
tag group, where the matrix format is the same as that of *abs_coef*
*/
ArrayOfMatrix& abs_coef_per_species(Workspace& ws) noexcept {
  return *static_cast<ArrayOfMatrix*>(ws[4]);
}

/*! Continuum / full model absorption model description parameter.

See the WSV `abs_cont_names' for a detailed description
of the allowed continuum models. There should be one string here
for each entry in `abs_cont_names'.See also the online
documentation in arts/doc/doxygen/html/continua_cc.html.
*/
ArrayOfString& abs_cont_models(Workspace& ws) noexcept {
  return *static_cast<ArrayOfString*>(ws[5]);
}

/*! Continuum / full model absorption tag names.

This variable should contain a list of tag names
of continuum and full models, respectively.
Associated with this WSV is the WSV
`abs_cont_models' which contains the specific model version of
each continuum / full model absorption tag and the WSV
`abs_cont_parameters' which should contain the continuum / full model
user defined parameters. The user defined parameters are only used when
the specified model is 'user'. See also the online documentation in
arts/doc/doxygen/html/continua_cc.html.

The following full water vapor models are implemented:
'H2O-MPM87': absorption model (line and continuum) according to
   H. J. Liebe,
   A contribution to modeling atmospheric millimeter-wave properties,
   Frequenz,  41, 1987, 31-36
   and
   H. J. Liebe and D. H. Layton,
   Millimeter-wave properties of the atmosphere:
   Laboratory studies and propagation modeling,
   U.S. Dept. of Commerce, National Telecommunications and Information
   Administration, Institute for Communication Sciences,
   325 Broadway, Boulder, CO 80303-3328, report 87224.
'H2O-MPM89': absorption model (line and continuum) according to
   H. J. Liebe,
 Int. J. Infrared and Millimeter Waves, 10(6), 1989, 631
'H2O-MPM93': absorption model (line and continuum) according to
   H. J. Liebe and G. A. Hufford and M. G. Cotton,
   Propagation modeling of moist air and suspended water/ice
   particles at frequencies below 1000 GHz,
   AGARD 52nd Specialists Meeting of the Electromagnetic Wave
   Propagation Panel,
 Palma de Mallorca, Spain, 1993, May 17-21
   (ftp.its.bldrdoc.gov/pub/mpm93/)
'H2O-CP98': absorption model (line and continuum) according to
   S. L. Cruz-Pol et al.,
 Radio Science, 33(5), 1319, 1998   (ece.uprm.edu/~pol/Atmosphere.html)
'H2O-PWR98': absorption model (line and continuum) according to
   P. W. Rosenkranz,
    Radio Science, 33(4),  919, 1998, Radio Science, 34(4), 1025, 1999
   (ftp: mesa.mit.edu/phil/lbl_rt).

The following full oxygen models are implemented:
'O2-MPM93': absorption model (line and continuum) according to
   H. J. Liebe and G. A. Hufford and M. G. Cotton,
   Propagation modeling of moist air and suspended water/ice
   particles at frequencies below 1000 GHz,
   AGARD 52nd Specialists Meeting of the Electromagnetic Wave
   Propagation Panel,
 Palma de Mallorca, Spain, 1993, May 17-21
   (ftp.its.bldrdoc.gov/pub/mpm93/)
'O2-PWR93': absorption model (line and continuum) according to
   P. W. Rosenkranz,
 Chapter 2, in M. A. Janssen,
   Atmospheric Remote Sensing by Microwave Radiometry
   John Wiley & Sons, Inc., 1993 (mesa.mit.edu/phil/lbl_rt)

The following continuum parameterizations are implemented:
H2O-H2O ('H2O-SelfContStandardType'):
   P. W. Rosenkranz,
   Radio Science, Vol. 33, No 4, Pages 919-928, 1998 and
   Radio Science, Vol. 34, No 4, Page 1025, 1999 (mesa.mit.edu/phil/lbl_rt)
H2O-air ('H2O-ForeignContStandardType'):
   P. W. Rosenkranz,
   Radio Science, Vol. 33, No 4, Pages 919-928, 1998 and
   Radio Science, Vol. 34, No 4, Page 1025, 1999 (mesa.mit.edu/phil/lbl_rt)
H2O-air ('H2O-ContMPM93'):
   H. J. Liebe and G. A. Hufford and M. G. Cotton,
   Propagation modeling of moist air and suspended water/ice
   particles at frequencies below 1000 GHz,
   AGARD 52nd Specialists Meeting of the Electromagnetic Wave
   Propagation Panel,
 Palma de Mallorca, Spain, 1993, May 17-21
   (ftp.its.bldrdoc.gov/pub/mpm93/)
O2-air ('O2-SelfContStandardType'):
   P. W. Rosenkranz,
   Chapter 2, in M. A. Janssen,
   Atmospheric Remote Sensing by Microwave Radiometry,
   John Wiley & Sons, Inc., 1993
   (mesa.mit.edu/phil/lbl_rt)
   and also described in
   H. J. Liebe and G. A. Hufford and M. G. Cotton,
   Propagation modeling of moist air and suspended water/ice
   particles at frequencies below 1000 GHz,
   AGARD 52nd Specialists Meeting of the Electromagnetic Wave
   Propagation Panel,
 Palma de Mallorca, Spain, 1993, May 17-21
   (ftp.its.bldrdoc.gov/pub/mpm93/)
N2-N2 ('N2-SelfContStandardType'):
   The functional form of Rosenkranz but with more input parameters.
   P. W. Rosenkranz,
   Chapter 2, in M. A. Janssen,
   Atmospheric Remote Sensing by Microwave Radiometry,
   John Wiley & Sons, Inc., 1993 (mesa.mit.edu/phil/lbl_rt)
N2-N2 ('N2-SelfContMPM93'):
   H. J. Liebe and G. A. Hufford and M. G. Cotton,
   Propagation modeling of moist air and suspended water/ice
   particles at frequencies below 1000 GHz,
   AGARD 52nd Specialists Meeting of the Electromagnetic Wave
   Propagation Panel, Palma de Mallorca, Spain, 1993, May 17-21
   (ftp.its.bldrdoc.gov/pub/mpm93/)
CO2-CO2 ('CO2-SelfContPWR93'):
   P. W. Rosenkranz,
   Chapter 2, in M. A. Janssen,
   Atmospheric Remote Sensing by Microwave Radiometry,
   John Wiley & Sons, Inc., 1993 (mesa.mit.edu/phil/lbl_rt)
CO2-N2 ('CO2-ForeignContPWR93'):
   P. W. Rosenkranz,
   Chapter 2, in M. A. Janssen,
   Atmospheric Remote Sensing by Microwave Radiometry,
   John Wiley & Sons, Inc., 1993 (mesa.mit.edu/phil/lbl_rt)

The following cloud absorption models are implemented:
Suspended water droplet ('liquidcloud-MPM93')
   absorption parameterization from the MPM93 model:
   H. J. Liebe and G. A. Hufford and M. G. Cotton,
   Propagation modeling of moist air and suspended water/ice
   particles at frequencies below 1000 GHz,
   AGARD 52nd Specialists Meeting of the Electromagnetic Wave
   Propagation Panel,
 Palma de Mallorca, Spain, 1993, May 17-21
   (ftp.its.bldrdoc.gov/pub/mpm93/)
Ice water droplet absorption ('icecloud-MPM93')
   parameterization from MPM93 model:
   H. J. Liebe and G. A. Hufford and M. G. Cotton,
   Propagation modeling of moist air and suspended water/ice
   particles at frequencies below 1000 GHz,
   AGARD 52nd Specialists Meeting of the Electromagnetic Wave
   Propagation Panel,
 Palma de Mallorca, Spain, 1993, May 17-21
   (ftp.its.bldrdoc.gov/pub/mpm93/)

The following rain extinction model is implemented:
Rain extinction parameterization ('rain-MPM93') from the
   MPM93 model, described in:
   H. J. Liebe,
   MPM - An Atmospheric Millimeter-Wave Propagation Model,
   Int. J. Infrared and Millimeter Waves, vol. 10(6),
   pp. 631-650, 1989;
   and based on:
   Olsen, R.L., D.V. Rogers, and D. B. Hodge,
   The aR^b relation in the calculation of rain attenuation,
   IEEE Trans. Antennas Propagat., vol. AP-26, pp. 318-329, 1978.
   IMPORTANT NOTE: rain-MPM93 parameterizes the EXTINCTION by rain,
    not just the absorption. Therefore it is not suitable for
    calculating thermal emission by rain!
    Please use rain-MPM93 only for calculation of attenuation.
*/
ArrayOfString& abs_cont_names(Workspace& ws) noexcept {
  return *static_cast<ArrayOfString*>(ws[6]);
}

/*! Continuum model parameters. See the WSV *abs_cont_names*

for a detailed description of the allowed continuum models. There
should be one parameter vector here for each entry in
*abs_cont_names*. See also the online documentation in
arts/doc/doxygen/html/continua_cc.html.
*/
ArrayOfVector& abs_cont_parameters(Workspace& ws) noexcept {
  return *static_cast<ArrayOfVector*>(ws[7]);
}

/*! Frequency interpolation order for absorption lookup table.

The interpolation order to use when interpolating the absorption
lookup table in frequency. This is in particular needed for
calculations with Doppler shift, so that absorption is interpolated to
the shifted frequency grid. One is linear interpolation, two
quadratic, and so on.

As a special case, order 0 in this particular case means no
interpolation. In that case f_grid must match exactly the grid inside
the lookup table. This is the global default value, set in
general.arts.
*/
Index& abs_f_interp_order(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[8]);
}

/*! HITRAN line mixing data to compute the relaxation matrix.

This variable holds HITRAN line mixing data
as per J. Lamouroux, L. Realia, X. Thomas, et al., J.Q.S.R.T. 151 (2015), 88-96
*/
HitranRelaxationMatrixData& abs_hitran_relmat_data(Workspace& ws) noexcept {
  return *static_cast<HitranRelaxationMatrixData*>(ws[9]);
}

/*! A list of spectral line data.
 */
ArrayOfAbsorptionLines& abs_lines(Workspace& ws) noexcept {
  return *static_cast<ArrayOfAbsorptionLines*>(ws[10]);
}

/*! A list of spectral line data for each tag.

Dimensions: [*abs_species*.nelem()][Depends on how many bands there are in
*abs_lines*]
*/
ArrayOfArrayOfAbsorptionLines& abs_lines_per_species(Workspace& ws) noexcept {
  return *static_cast<ArrayOfArrayOfAbsorptionLines*>(ws[11]);
}

/*! An absorption lookup table.

It holds an absorption lookup table, as well as all information that
is necessary to use the table to extract absorption. Extraction
routines are implemented as member functions.

It has quite a complicated structure. For details see the Arts User
Guide section "The gas absorption lookup table" or the source code
documentation in gas_abs_lookup.h.
*/
GasAbsLookup& abs_lookup(Workspace& ws) noexcept {
  return *static_cast<GasAbsLookup*>(ws[12]);
}

/*! Flag to indicate whether *abs_lookupAdapt* has already been
called.

Values: 0=false, 1=true.
*/
Index& abs_lookup_is_adapted(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[19]);
}

/*! Nonlinear species for absorption lookup table generation.

A list of absorption species that should be treated non-linearly.
This means that the H2O VMR should be varied when calculating the
lookup table for those species.

A typical example is for this to containt the Rosenkranz full
absorption model species for water vapor and oxygen
(["H2O-PWR98", "O2-PWR93"]).

See user guide and online documentation of *abs_lookupCalc*
for more details and usage examples.
*/
ArrayOfArrayOfSpeciesTag& abs_nls(Workspace& ws) noexcept {
  return *static_cast<ArrayOfArrayOfSpeciesTag*>(ws[13]);
}

/*! The interpolation order to use when interpolating absorption between
the H2O values given by *abs_nls_pert*.

This is used by methods extracting absorption coefficients
from the lookup table, and by methods setting up
parameters for lookup table generation. Has a
default value, which is set in general.arts.

Note that the number of points used in the interpolation scheme is
interpolation order + 1 (e.g., two for first order interpolation).
*/
Index& abs_nls_interp_order(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[15]);
}

/*! Fractional perturbations for the nonlinear species in the absorption
lookup table.

This is a vector of fractional perturbations that should contain 1
(the unperturbed reference profile). A value of 0 may lead to error
messages from some absorption routines, so a possible content for this
variable is: [1e-24, 1, 2].
(This is similar to *abs_t_pert*, but multiplicative, not additive.)
*/
Vector& abs_nls_pert(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[14]);
}

/*! NLTE temperatures or ratios to be used for the calculation of
absorption coefficients.

In contrast to the global *nlte_field*, this is just a matrix. Any
absorption method should check that the columns of this vector is the
same as that of *abs_p*

Dimension: [nltes, 1, 1, p_grid] or [ 0, 0, 0, 0 ]

Unit: K
*/
EnergyLevelMap& abs_nlte(Workspace& ws) noexcept {
  return *static_cast<EnergyLevelMap*>(ws[24]);
}

/*! List of pressures to be used for the calculation of absorption
coefficients.

This can be copied from the global *p_grid*, but could also be
different.

Any absorption method should check that the length of this vector
is the same as that of *abs_t*

Dimension: [p_grid]

Unit: Pa
*/
Vector& abs_p(Workspace& ws) noexcept { return *static_cast<Vector*>(ws[20]); }

/*! The interpolation order to use when interpolating absorption
between pressure levels.

This is used by methods extracting absorption coefficients
from the lookup table, and by methods
setting up parameters for lookup table generation. Has a
default value, which is set in general.arts.

Note that the number of points used in the interpolation scheme is
interpolation order + 1 (e.g., two for first order interpolation).
*/
Index& abs_p_interp_order(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[16]);
}

/*! Tag groups for gas absorption.

This is an array of arrays of SpeciesTag tag definitions. It defines the
available tag groups for the calculation of scalar gas absorption
coefficients.  See online documentation of method *abs_speciesSet* for
more detailed information how tag groups work and some examples.
*/
ArrayOfArrayOfSpeciesTag& abs_species(Workspace& ws) noexcept {
  return *static_cast<ArrayOfArrayOfSpeciesTag*>(ws[21]);
}

/*! Indices of active absorption species.

This variable selects, which absorption species are active in
*abs_xsec_agenda*.

Dimension: A vector with one element for every active species, at max
           same number of elements as abs_species.
*/
ArrayOfIndex& abs_species_active(Workspace& ws) noexcept {
  return *static_cast<ArrayOfIndex*>(ws[22]);
}

/*! List of temperatures to be used for the calculation of absorption
coefficients.

In contrast to the global *t_field*, this is just a vector. Any
absorption method should check that the length of this vector is the
same as that of *abs_p*

Dimension: [p_grid]

Unit: K
*/
Vector& abs_t(Workspace& ws) noexcept { return *static_cast<Vector*>(ws[23]); }

/*! The interpolation order to use when interpolating absorption between
the temperature values given by *abs_t_pert*.

This is used by methods
extracting absorption coefficients from the lookup table, and by
methods setting up parameters for lookup table generation. Has a
default value, which is set in general.arts.

Note that the number of points used in the interpolation scheme is
interpolation order + 1 (e.g., two for first order interpolation).
*/
Index& abs_t_interp_order(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[18]);
}

/*! Temperature perturbations for the absorption lookup table.

This is a vector containing temperature perturbations (in Kelvin) that
should be added to the reference temperature profile. (Similar to
*abs_nls_pert*, but additive, not multiplicative.) Should normally
contain 0, to include the reference profile itself. Example content:
[-5, 0, 5].
*/
Vector& abs_t_pert(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[17]);
}

/*! Total absorption vector.

This variable contains the absorption coefficient vector which
is used in the RTE calculation. It is the physical absorption which
includes particle absorption for all considered scattering elements as
well as gaseous absorption for all selected gaseous species.
The vector is calculated by *opt_prop_bulkCalc*
The dimension of the variable adapts to *stokes_dim*.

See ARTS user guide (AUG) for further information. Use the index to find
where this variable is discussed. The variable is listed as a subentry
to "workspace variables".

Usage:      Output of *opt_prop_bulkCalc*

Unit:       m^2

Dimensions: [f_grid, stokes_dim]
*/
StokesVector& abs_vec(Workspace& ws) noexcept {
  return *static_cast<StokesVector*>(ws[25]);
}

/*! Absorption vectors of the scattering elements.

This variable contains the elements of the absorption vector of the
individual scattering elements. It is calculated in the agenda
*spt_calc_agenda*.

See ARTS user guide (AUG) for further information.

Usage:      Input and Output of the method abs_vec_sptCalc

Unit:        m^2

Dimensions: [number of scattering elements, stokes_dim]
*/
ArrayOfStokesVector& abs_vec_spt(Workspace& ws) noexcept {
  return *static_cast<ArrayOfStokesVector*>(ws[26]);
}

/*! The VMRs (unit: absolute number) on the abs_p grid.

Dimensions: [tag_groups.nelem(), abs_p.nelem()]
*/
Matrix& abs_vmrs(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[27]);
}

/*! Agenda to calculate scalar gas absorption cross sections.
 */
Agenda& abs_xsec_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[29]);
}

/*! OK-flag for *abs_xsec_agenda*.

Set by *abs_xsec_agenda_checkedCalc*.
*/
Index& abs_xsec_agenda_checked(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[28]);
}

/*! Absorption cross sections for the attenuation.

This variable contains absorption cross section xsec individually for
each tag group. The Array contains one matrix for each tag group, the
matrix format is the same as that of abs_coef.

Dimensions: [abs_species](f_grid, abs_p)

Unit:       m^2 (alpha = xsec * n * VMR),
            where n is total density.
*/
ArrayOfMatrix& abs_xsec_per_species(Workspace& ws) noexcept {
  return *static_cast<ArrayOfMatrix*>(ws[30]);
}

/*! Index of the current agenda in *ArrayOfAgenda*.

This is set during the execution of an agenda from an *ArrayOfAgenda*.
It indicates the index of the current agenda inside the array.

Unit:  Integer value.
*/
Index& agenda_array_index(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[31]);
}

/*! The dimensionality of the antenna pattern (1-2).

A dimensionality of 1 means that only the respons variation in the
zenith direction is considered. The provided respons shall then be the
integrated in the azimuth direction. For 2D, the respons of the
antenna has both a zenith and azimuth variation.

Usage: Set by the user.

Unit:  Integer value [1-2].
*/
Index& antenna_dim(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[32]);
}

/*! The relative line-of-sight of each antenna pattern.

This variable describes the line-of-sight of the individual antennae
relative to *sensor_los*. If each measurement block corresponds to
a single antenna pattern, the normal choice is to set the angle(s) of
this variable to zero.

The first column holds the relative zenith angle. This column is
mandatory for all atmospheric dimensionalities. For 3D, there can
also be a second column, giving relative azimuth angles. If this
column is not present (for 3D) zero azimuth off-sets are assumed.

See further the ARTS user guide (AUG). Use the index to find where
this variable is discussed. The variable is listed as a subentry to
"workspace variables".

Usage: Set by the user.

Unit:  [ degrees, degrees ]

Size:  [ number of antennae, 1 or 2 ]
*/
Matrix& antenna_dlos(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[33]);
}

/*! The antenna pattern/response.

This WSV describes the antenna response as a function of polarisation
(pol), frequencue (f), zenith angle (za) and azimuth angle (aa).

Polarisation dimension: If this dimension has size 1, the data are
applied for all polarisations of concern. The data are otherwise used
in sequential order. This signifies that, in general, the first
polarisation "layer" corresponds to the first stokes dimension
etc. An exception is if a polarisation rotation has been applied. In
any case, it is up to the user to ensure that polarisations are
consistently defined.

Frequency dimension: If this dimension has size 1, the data are
applied for all frequencies of concern. The given frequency must be
inside the frequency range of concern. A linear interpolation is
otherwise applied.

Zenith angle dimension: This dimension must always have a size >= 2
The response outside covered grid range is treated as zero. If
*antenna_dim* equals 1, the data should correspond to the response
integrated in the azimuthal direction.

Azimuth angle dimension: If *antenna_dim* equals 1, this dimension
must have size 1. A size >= 2 is otherwise required. The response
outside covered grid range is treated as zero.

Usage: Set by the user.

Dimensions:
   GriddedField4:
      ArrayOfString field_names[N_pol]
      Vector f_grid[N_f]
      Vector za_grid[N_za]
      Vector aa_grid[N_aa]
      Tensor4 data[N_pol][N_f][N_za][N_aa]
*/
GriddedField4& antenna_response(Workspace& ws) noexcept {
  return *static_cast<GriddedField4*>(ws[34]);
}

/*! A compact set of atmospheric fields on a common set of grids.

Data is supposed to contain basic atmsopheric fields for a RT
calculation, i.e., temperature, altitude, and gas VMRs. It can
furthermore contain fields describing scattering species like mass
content, mass flux, number density of diverse scattering species.

VMR fields are unitless, scattering species fields are supposed to be
in SI units (i.e. kg/m3 for mass contents, kg/m2/s for mass flux,
1/m3 for number densities).

The data are stored in a *GriddedField4*.

The first field in the matrix (i.e., first matrix column) has to be
atmospheric pressure. Apart from this, the order of the fields is
free. Field content (apart from pressure) is identified by their
given field name tag. Furthermore, absorption species (e.g. VMR)
fields and scattering species fields are related to *abs_species*
and *scat_species* entries, respectively, by their field name tags.
The tags must exhibit the following structure:

0) species identifier:
   Fields, supposed to be sorted into *vmr_field*, must be headed the
   tag 'abs_species'. Names of scattering species fields likewise must
   be headed by the 'scat_species' tag. Temperature and altitude
   fields do not hold any heading tag.
1) species name:
   The (core) name of the field: 'T' for temperature, 'z' for
   altitude, the absorption species name (e.g. 'H2O, 'O3', etc.) for
   absorption species, the scattering species name (e.g. 'IWC') for
   scattering species. For scattering species, this part is matched
   against the scattering species name part of the *scat_species*
   tags.
2) field type:
   This has to be given for scattering species only, indicating the
   type of the scattering species fields, i.e. 'mass_density',
   'mass_flux', 'number_density', 'mean_mass'.
Dashes ('-') serve as delimiter, separating the elements of each
field name tag.

Usage: Used inside batch calculations, to hold successive atmospheric
       states from an *ArrayOfGriddedField4*.

Dimensions:
   GriddedField4:
      ArrayOfString field_names[N_fields]
      Vector p_grid[N_p]
      Vector lat_grid[N_lat]
      Vector lon_grid[N_lon]
      Tensor4 data[N_fields][N_p][N_lat][N_lon]
*/
GriddedField4& atm_fields_compact(Workspace& ws) noexcept {
  return *static_cast<GriddedField4*>(ws[38]);
}

/*! OK-flag for atmospheric grids and (physical) fields.

The variable flags that clear-sky part of the atmosphere is
defined in formally correct way. Example on problems captured
include that the size of an atmospheric fields does not match the
length of the atmospheric grids, and physically incorrect data such
as negative temperatures.

Note that *z_field* is not covered by this variable, it is instead
treated to be part of the geometrical considerations where the ok-flag
is denoted as *atmgeom_checked*. The cloudbox is covered by
*cloudbox_checked*.

Shall be set by *atmfields_checkedCalc*. See that WSMs for treated
WSVs. Only the value 1 is taken as OK.
*/
Index& atmfields_checked(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[36]);
}

/*! OK-flag for the geometry of the model atmosphere.

The variable flags that reference ellipsoid, the surface and *z_field*
contain formally correct values. Includes for example, that *z_field*
holds strictly increasing values at each geographical position.

See also *atmfields_checked*.

Shall be set by *atmgeom_checkedCalc*. Only the value 1 is taken
as OK.
*/
Index& atmgeom_checked(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[37]);
}

/*! The atmospheric dimensionality (1-3).

This variable defines the complexity of the atmospheric structure.
The dimensionality is given by an integer between 1 and 3, where 1
means 1D etc. This is the master variable for the atmospheric
dimensionality, variables which size changes with the dimensionality
are checked to match this variable.

Methods adapt automatically to this variable. That is, it should
not be needed to change any methods if the dimensionality is
changed. However, not all methods are working for higher dimensions.

Usage: Set by the user.

Unit: Integer value.
*/
Index& atmosphere_dim(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[35]);
}

/*! Averaging kernel matrix.

This matrix is the partial derivative of the retrieved state vector
with respect to the measurement vector (*y*).

Usage: Used and set by inversion methods.
*/
Matrix& avk(Workspace& ws) noexcept { return *static_cast<Matrix*>(ws[39]); }

/*! The response of each backend channel.

The response is given as an *ArrayOfGriddedField1*. The grid consists of
relative frequencies. These relative frequencies are added to
*f_backend* to obtain the absolute frequency for each response value.
The actual data are the response at each frequency grid point.

There are here two options. If the array has length 1, the same
response is applied for all channels. Accordingly, this assumes that
all channels have the same response function. The second option is to
specify the response for each channel seperately. This signifies that
the *backend_channel_response* array has either 1 or n elements, where
n is the length of *f_backend*

Usage: Set by the user.

Size:  Array[N_ch]
       GriddedField1
        [N_f]
       [N_f]
*/
ArrayOfGriddedField1& backend_channel_response(Workspace& ws) noexcept {
  return *static_cast<ArrayOfGriddedField1*>(ws[40]);
}

/*! As *backend_channel_response* but describes an instrument with
muliple mixer/receiver chains.

See *f_backend_multi* for when to use this variable and size
constraints.

Usage: Set by the user.
 */
ArrayOfArrayOfGriddedField1& backend_channel_response_multi(
    Workspace& ws) noexcept {
  return *static_cast<ArrayOfArrayOfGriddedField1*>(ws[41]);
}

/*! An array of identifiers for bands.

Used by line mixing calculations to identify which bands to match to the
line database.
*/
ArrayOfQuantumIdentifier& band_identifiers(Workspace& ws) noexcept {
  return *static_cast<ArrayOfQuantumIdentifier*>(ws[43]);
}

/*! An array of compact atmospheric states.

This is used to hold a set of *atm_fields_compact* for batch
calculations. For further information see *atm_fields_compact*.
*/
ArrayOfGriddedField4& batch_atm_fields_compact(Workspace& ws) noexcept {
  return *static_cast<ArrayOfGriddedField4*>(ws[42]);
}

/*! An array of *cloudbox_limits*.

This is used to hold a set of *cloudbox_limits* for batch
calculations.
*/
ArrayOfArrayOfIndex& batch_cloudbox_limits(Workspace& ws) noexcept {
  return *static_cast<ArrayOfArrayOfIndex*>(ws[44]);
}

/*! An array of compact pnd states.

This is used to hold a set of 1D *pnd_field* for batch
calculations.
*/
ArrayOfTensor4& batch_pnd_fields(Workspace& ws) noexcept {
  return *static_cast<ArrayOfTensor4*>(ws[45]);
}

/*! Definition of backend frequency response, link to *f_grid*.

The WSV is used to describe the frequency response of backend channels
together with the accompanying WSV *channel2fgrid_weights*.

This WSV links each channel to the elements of *f_grid*. In short it
lists what elements of *f_grid* that are relevant for each channel.

More precisely, the first dimension gives the number of output channels.
Each ArrayOfIndex gives the index of the values in *f_grid* associated
with the channel of concern. For a pure double-sideband receiver, where
there is one monochromatic frequency per passband, this argument could
look like: [[0,5],[1,4],[2,3],[7,8],[7,8]].
*/
ArrayOfArrayOfIndex& channel2fgrid_indexes(Workspace& ws) noexcept {
  return *static_cast<ArrayOfArrayOfIndex*>(ws[46]);
}

/*! Definition of backend frequency response, weighting of *f_grid*.

The WSV is used to describe the frequency response of backend channels
together with the accompanying WSV *channel2fgrid_indexes*.

This WSV shall have excatly the same sizes as *channel2fgrid_indexes*.
Each element gives the weight to be assigned to the associated
monochromatic frequency.
*/
ArrayOfVector& channel2fgrid_weights(Workspace& ws) noexcept {
  return *static_cast<ArrayOfVector*>(ws[47]);
}

/*! OK-flag for variables associated with the cloudbox.

This variable flags that cloudbox variables are defined in a formally
and practically correct way. For example, that there is sufficient
space between the cloudbox and edges of the model atmosphere (for
2D and 3D). Pure clear-sky variables are covered by
*atmfields_checked* (and *atmgeom_checked*).

Relevant checks are performed by *cloudbox_checkedCalc. Only the
value 1 is taken as OK.
*/
Index& cloudbox_checked(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[48]);
}

/*! The spectral radiance field inside the cloudbx.

This variable is used to store the radiance field inside the cloud
box, probably determined by a scattering solver method.

That is, this variable matches *spectral_radiance_field* but holds
a field that is restricted to the cloud box.

Unit: W / (m^2 Hz sr) for each Stokes component.

 Size: [f_grid,
       p_grid,
       lat_grid,
       lon_grid,
       za_grid,
       aa_grid,
       stokes_dim ]

Note: For 1D, the size of the latitude, longitude and azimuth
dimension (N_aa) are all 1.
*/
Tensor7& cloudbox_field(Workspace& ws) noexcept {
  return *static_cast<Tensor7*>(ws[49]);
}

/*! Monochromatic radiation field inside the cloudbox.

This variable is used to store the monochromatic radiation field
inside the cloudbox which is found by an iterative solution (DOIT).
Refer to AUG for further information.

Usage: Method output.

Unit: W / (m^2 Hz sr) for each Stokes component.

Size: [(cloudbox_limits[1] - cloudbox_limits[0]) +1,
       (cloudbox_limits[3] - cloudbox_limits[2]) +1,
       (cloudbox_limits[5] - cloudbox_limits[4]) +1,
        N_za, N_aa, N_i ]

Note: For 1D, the size of the azimuth angle dimension (N_aa) is
always 1.
*/
Tensor6& cloudbox_field_mono(Workspace& ws) noexcept {
  return *static_cast<Tensor6*>(ws[50]);
}

/*! As *cloudbox_field_mono* but from previous iteration.

This variable is used to store the intensity field inside the
cloudbox while performing the iteration. One has to store the
intensity field of the previous iteration to be able to do the
convergence test after each iteration.
Refer to AUG for more information.

Usage: Method output.

Unit: W / (m^2 Hz sr) for each Stokes component.

Size: [(cloudbox_limits[1] - cloudbox_limits[0]) +1,
       (cloudbox_limits[3] - cloudbox_limits[2]) +1,
       (cloudbox_limits[5] - cloudbox_limits[4]) +1,
        N_za, N_aa, N_i ]
*/
Tensor6& cloudbox_field_mono_old(Workspace& ws) noexcept {
  return *static_cast<Tensor6*>(ws[51]);
}

/*! The limits of the cloud box.

This variable defines the extension of the cloud box. The cloud box
is defined to be rectangular in the used coordinate system, with
limits exactly at points of the involved grids. This means, for
example, that the vertical limits of the cloud box are two pressure
levels. For 2D, the angular extension of the cloud box is between
two points of the latitude grid, and likewise for 3D but then also
with a longitude extension between two grid points. The latitude and
longitude limits for the cloud box cannot be placed at the end
points of the corresponding grid as it must be possible to calculate
the incoming intensity field.

The variable *cloudbox_limits* is an array of index value with
length twice *atmosphere_dim*. For each dimension there is a lower
limit and an upper limit. The order of the dimensions is as usual
pressure, latitude and longitude. The upper limit index must be
greater then the lower limit index. For example,
*cloudbox_limits* = [0 5 4 11 4 11] means that cloud box extends
between pressure levels 0 and 5, and latitude and longitude points 4
and 11.

If *cloudbox_on* = 0, the content of this variable is neglected, but
it must be initiated to some dummy values.

See further the ARTS user guide (AUG). Use the index to find where
this variable is discussed. The variable is listed as a subentry to
"workspace variables".

Usage: Set by the user, either directly or using a method
       checking the extension of scattering particles.

Unit:  Index values.

Size:  [ 2 * atmosphere_dim ]
*/
ArrayOfIndex& cloudbox_limits(Workspace& ws) noexcept {
  return *static_cast<ArrayOfIndex*>(ws[52]);
}

/*! Flag to activate the cloud box.

Scattering calculations are confined to a part of the atmosphere
denoted as the cloud box. The extension of the cloud box is given by
*cloudbox_limits*. This variable tells methods if a cloud box is
activated or not.

See further the ARTS user guide (AUG). Use the index to find where
this variable is discussed. The variable is listed as a subentry to
"workspace variables".

Usage: Set by the user.

Unit:  Boolean.
*/
Index& cloudbox_on(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[53]);
}

/*! An array of coefficients for effective collisions
 */
ArrayOfArrayOfGriddedField1& collision_coefficients(Workspace& ws) noexcept {
  return *static_cast<ArrayOfArrayOfGriddedField1*>(ws[182]);
}

/*! An array of quantum identifiers for finding collisional rates
in *collision_coefficients*
*/
ArrayOfQuantumIdentifier& collision_line_identifiers(Workspace& ws) noexcept {
  return *static_cast<ArrayOfQuantumIdentifier*>(ws[181]);
}

/*! Complex refractive index (n) data.

The variable works as a lookup-table of complex refractive index.
The matter type (water, ice ...) is unspecified, it is up to the
user to fill the variable with data for the expected matter.
This variable type can be used to describe n of both the surface and
atmospheric particles. For the surface, a dedicated variable exists:
*surface_complex_refr_index*.

The column dimension has always size 2, where the first and second
column holds the real and imaginary part of n, respectively. The row
dimension matches temperature, and the page dimension is frequency.
Both the temperature and frequency dimensions grids are allowed to
have length 1, which is interpreted as n being constant in that
dimension.

When mapping these data to the required frequencies and temperatures
a bi-linear interpolation is applied.

Unit:       -

Dimensions:
      Vector f_grid[N_f]
      Vector T_grid[N_T]
      ArrayOfString Complex[2]
      Tensor3 data[N_f][N_T][2]
*/
GriddedField3& complex_refr_index(Workspace& ws) noexcept {
  return *static_cast<GriddedField3*>(ws[54]);
}

/*! Holds a list of counts, any counts.
 */
ArrayOfIndex& counts(Workspace& ws) noexcept {
  return *static_cast<ArrayOfIndex*>(ws[55]);
}

/*! Holds matrices used to set blocks in *covmat_sx* and *covmat_se*.

The matrix contained in this block will be added to the blocks in
in *covmat_sx* and *covmat_se* by the corresponding WSMs. Its dimensions
must agree with gridpoints of the correlated retrieval quantities.
Usage:   Used by the retrievalAdd functions.
*/
Sparse& covmat_block(Workspace& ws) noexcept {
  return *static_cast<Sparse*>(ws[56]);
}

/*! Holds matrices used to set the inverse blocks in *covmat_sx* and
*covmat_se*.

The matrix contained in this block will be used as the inverse of the matrix
contained in covmat_block.

Usage:   Used by the retrievalAdd functions.
*/
Sparse& covmat_inv_block(Workspace& ws) noexcept {
  return *static_cast<Sparse*>(ws[57]);
}

/*! Covariance matrix for observation uncertainties.

This matrix (Se) describes the uncertainty of the measurement vector (*y*),
and can be writtenn as
   Se = Seps + Kb * Sb * Kb',
where Seps describes direct measurement errors (such as thermal noise),
Kb is Jacobian for forward model parameters, and Sb describes the uncertainty
of the forwatrd model parameters.

Usage:   Used by inversion methods.

Dimensions:
     [ y, y ]
*/
CovarianceMatrix& covmat_se(Workspace& ws) noexcept {
  return *static_cast<CovarianceMatrix*>(ws[58]);
}

/*! Covariance matrix for measurement uncertainties

Dimensions:
     [*ybatch*.nelem()][ *ybatch*[i], *ybatch*[i] ]
*/
ArrayOfMatrix& covmat_sepsbatch(Workspace& ws) noexcept {
  return *static_cast<ArrayOfMatrix*>(ws[59]);
}

/*! Covariance matrix describing the retrieval error due to uncertainties of
the observation system.

That is: So = G * Se * G', where G is the gain matrix (*dxdy*).

Usage: Set by the covmat_soCalc workspace method to characterize the error.
of a successful OEM calculation.

Dimensions:
    [x,x]
*/
Matrix& covmat_so(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[61]);
}

/*! Covariance matrix describing the retrieval error due to smoothing.

That is: Ss = (A-I) * Sx * (A-I)', where A is the averaging kernel matrix
(*avk*).

Usage: Set by the covmat_ssCalc workspace method to characterize the.
errors of a successful OEM calculation.
Dimensions:
    [x,x]
*/
Matrix& covmat_ss(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[62]);
}

/*! Covariance matrix of a priori distribution

This covariance matrix describes the Gaussian a priori distribution
for an OEM retrieval. It is represented using a symmetric block matrix.
covmat_sx can be used in two ways: Either with a block for each retrieval
quantity or with a single block containing the full covariance matrix.

Using a single block for each retrieval quantity has is advantageous for
if the retrieval quantities are assumed to be independent. In this case,
the covariance blocks can be added separately for each quantity and will
allow optimizing matrix multiplications and inverses required for the OEM
calculation.

The other case of using a single-block covariance matrix is supported
for convenience as well.

Usage:   Used by inversion methods.

Dimensions:
     [ x, x ]
*/
CovarianceMatrix& covmat_sx(Workspace& ws) noexcept {
  return *static_cast<CovarianceMatrix*>(ws[60]);
}

/*! The partial derivatives of the matrix of total scalar absorption
coefficients.

Contains the derivative of the gas absorption summed over all
species as a function of *f_grid* and *abs_p*, i.e., for a single
atmospheric profile for some parameter.

This variable is not used explicitly in a standard calculation, where
absorption comes from the lookup table *abs_lookup*. However, it is
useful for testing the methods that actually calculate line-by-line
absorption, which have this variable as output. These methods are
called internally by the method *abs_lookupCalc*, which generates
the lookup table.

Dimensions: [n_quantities][f_grid, abs_p]

Unit: 1/m/quantity
*/
ArrayOfMatrix& dabs_coef_dx(Workspace& ws) noexcept {
  return *static_cast<ArrayOfMatrix*>(ws[63]);
}

/*! Derivative of *abs_xsec_per_species* with respect to retrieval
quantities.

The variable gives the derivative of *abs_xsec_per_species* with
respect to some variables (but not all jacobian variables). Handled
are only variables that are involved in xsec and cannot be
calculated at transmission level

Usage:      Output of *abs_xsec_agenda*.

Dimensions:
     [*abs_species*][n_quantities][*f_grid*, *abs_p* ]
*/
ArrayOfArrayOfMatrix& dabs_xsec_per_species_dx(Workspace& ws) noexcept {
  return *static_cast<ArrayOfArrayOfMatrix*>(ws[64]);
}

/*! Derivative of *iy* with respect to retrieval quantities.

The variable gives the derivative if *iy* with respect to some
variables (but not all jacobian variables). Handled are only variables
affecting monochromatic pencil beam radiances where an (semi-)
analytical expression can be applied (and that this calculation way
has been selected when the jacobian was been set-up).

The values in *diy_dx* considers the retrieval unit selected (such as
"nd"), but no transformations are applied.

Usage:      Output of *iy_main_agenda*.

Dimensions:
     [n_quantities][ n_retrieval_points, f_grid, stokes_dim ]
*/
ArrayOfTensor3& diy_dx(Workspace& ws) noexcept {
  return *static_cast<ArrayOfTensor3*>(ws[72]);
}

/*! NLTE partial derivatives output is two parts:  S*dB/dx+dS/dx*B.
This should contain the latter term for one point in the atmosphere
(one set of pressure, temperature, zn magnetic field, and VMR values)
with respect to one of of the input parameters.

Dimensions: [ quantities ] [nza, naa, nf, stokes_dim] or [0]

Unit: 1/m/jacobian_quantity
*/
ArrayOfStokesVector& dnlte_dx_source(Workspace& ws) noexcept {
  return *static_cast<ArrayOfStokesVector*>(ws[78]);
}

/*! Agenda defining the calculations to perform for each batch case.
 */
Agenda& dobatch_calc_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[65]);
}

/*! Batch of radiation fields.

Each element of *dobatch_cloudbox_field* corresponds to a radiation field.
See further *DOBatchCalc*.

Usage: Most commonly produced by *DOBatchCalc*.

Unit:  See *cloudbox_field*.

Dimensions: Number of array elements equals number of batch cases.
*/
ArrayOfTensor7& dobatch_cloudbox_field(Workspace& ws) noexcept {
  return *static_cast<ArrayOfTensor7*>(ws[66]);
}

/*! Batch of irradiance fields.

Each element of *dobatch_irradiance_field* corresponds to a irradiance field.
See further *DOBatchCalc*.

Usage: Most commonly produced by *DOBatchCalc*.

Unit:  See *irradiance_field*.

Dimensions: Number of array elements equals number of batch cases.
*/
ArrayOfTensor4& dobatch_irradiance_field(Workspace& ws) noexcept {
  return *static_cast<ArrayOfTensor4*>(ws[68]);
}

/*! Batch of radiance fields.

Each element of *dobatch_radiance_field* corresponds to a radiance field.
See further *DOBatchCalc*.

Usage: Most commonly produced by *DOBatchCalc*.

Unit:  See *radiance_field*.

Dimensions: Number of array elements equals number of batch cases.
*/
ArrayOfTensor5& dobatch_radiance_field(Workspace& ws) noexcept {
  return *static_cast<ArrayOfTensor5*>(ws[67]);
}

/*! Batch of spectral irradiance fields.

Each element of *dobatch_spectral_irradiance_field* corresponds to a
spectral irradiance field.
See further *DOBatchCalc*.

Usage: Most commonly produced by *DOBatchCalc*.

Unit:  See *spectral_irradiance_field*.

Dimensions: Number of array elements equals number of batch cases.
*/
ArrayOfTensor5& dobatch_spectral_irradiance_field(Workspace& ws) noexcept {
  return *static_cast<ArrayOfTensor5*>(ws[69]);
}

/*! Flag for the convergence test.

This variable is initialized with 0 inside the method
*cloudbox_field_monoIterate*.
If after an iteration the convergence test is fulfilled, 1 is
assigned which means that the iteration is completed.

Usage: Method output.
*/
Index& doit_conv_flag(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[80]);
}

/*! Agenda executing the DOIT convergence test.
 */
Agenda& doit_conv_test_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[81]);
}

/*! Flag to determine if *DoitInit* was called.

This flag is checked by *DoitCalc* to make sure that
*DoitInit* was called before.
*/
Index& doit_is_initialized(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[82]);
}

/*! Counter for number of iterations.

This variable holds the number of iterations
while solving the VRTE using the DOIT method.
*/
Index& doit_iteration_counter(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[83]);
}

/*! Agenda performing monochromatic DOIT calculation.
 */
Agenda& doit_mono_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[84]);
}

/*! Agenda performing the DOIT cloudbox radiative transfer update.
 */
Agenda& doit_rte_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[85]);
}

/*! Scattered field inside the cloudbox.

This variable holds the value of the scattering integral for all
points inside the cloudbox. For more information refer to AUG.

Usage: Input to *cloudbox_fieldUpdate...*.

Unit: W / (m^2 Hz sr) for each Stokes component.

Size: [(cloudbox_limits[1] - cloudbox_limits[0]) +1,
       (cloudbox_limits[3] - cloudbox_limits[2]) +1,
       (cloudbox_limits[5] - cloudbox_limits[4]) +1,
        N_za, N_aa, N_i ]
*/
Tensor6& doit_scat_field(Workspace& ws) noexcept {
  return *static_cast<Tensor6*>(ws[87]);
}

/*! Agenda calculating the scattering integral field in DOIT.
 */
Agenda& doit_scat_field_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[86]);
}

/*! Optimized zenith angle grid.

Output of the method *doit_za_grid_optCalc*.

Usage:   Output of *doit_za_grid_optCalc*

Unit:    degrees
*/
Vector& doit_za_grid_opt(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[88]);
}

/*! Number of equidistant grid points of the zenith angle grid.

Grid points are defined from 0 to 180 deg, for the scattering
integral calculation.

Usage: Output of *DOAngularGridsSet*.
*/
Index& doit_za_grid_size(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[89]);
}

/*! Flag for interplation method in zenith angle dimension.

0 - linear interpolation
1 - cubic interpolation

Usage: Set by user in *doit_za_interpSet*.
*/
Index& doit_za_interp(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[90]);
}

/*! Partial derivates of *pnd_data*.

The variable gives the particle derivate of *pnd_data* with respect
to the quantities set in *dpnd_data_dx_names*.

Dimensions: [ n_quantities, n_points, n_scattering_elements ]
*/
Tensor3& dpnd_data_dx(Workspace& ws) noexcept {
  return *static_cast<Tensor3*>(ws[73]);
}

/*! Selection of partial derivatives of *pnd_data*.

This variable tells an element in *pnd_agenda_array* for which
quantities partial derivatives shall be calculated.

Dimensions: [ n_quantities ]
*/
ArrayOfString& dpnd_data_dx_names(Workspace& ws) noexcept {
  return *static_cast<ArrayOfString*>(ws[74]);
}

/*! Partial derivatives of *pnd_field*.

The variable gives the particle derivative of *pnd_field* with respect
to scattering species variables included in *jacobian_quantities*.

The length of this array shall match the size of *jacobian_quantities*.
For retrieval quantities that are not scattering species, the matching
Tensor4 is of no relevance and must be set to be empty.

Dimensions: [n_quantities][ n_scattering_elements, n_p, n_lat, n_lon ]
*/
ArrayOfTensor4& dpnd_field_dx(Workspace& ws) noexcept {
  return *static_cast<ArrayOfTensor4*>(ws[75]);
}

/*! Partial derivative of absorption coefficients.

This contains the partial derivative of absorption coefficients for
one point in the atmosphere (one set of pressure, temperature, znmagnetic field,
and VMR values) with respect to one of the input parameters.

Dimension: [ n_quantities ] [naa, nza, nf, f(stokes_dim)]

*jacobian_quantities* should be used to set the input variable for
partial derivation

Unit: 1/m/jacobian_quantity
*/
ArrayOfPropagationMatrix& dpropmat_clearsky_dx(Workspace& ws) noexcept {
  return *static_cast<ArrayOfPropagationMatrix*>(ws[76]);
}

/*! Partial derivates of *psd_data*.

The variable gives the particle derivate of *psd_data* with respect
to the quantities set in *dpnd_data_dx_names*.

Dimensions: [ n_quantities, n_points, n_scattering_elements ]
*/
Tensor3& dpsd_data_dx(Workspace& ws) noexcept {
  return *static_cast<Tensor3*>(ws[77]);
}

/*! The partial derivatives of the matrix of total scalar NLTE source term.

Contains the derivative of the NLTE source term summed over all
species as a function of *f_grid* and *abs_p*, i.e., for a single
atmospheric profile for some parameter.

This variable is not used explicitly in a standard calculation, where
absorption comes from the lookup table *abs_lookup*. However, it is
useful for testing the methods that actually calculate line-by-line
absorption, which have this variable as output. These methods are
called internally by the method *abs_lookupCalc*, which generates
the lookup table.

Dimensions: [n_quantities][f_grid, abs_p]

Unit: 1/m/quantity
*/
ArrayOfMatrix& dsrc_coef_dx(Workspace& ws) noexcept {
  return *static_cast<ArrayOfMatrix*>(ws[70]);
}

/*! Derivative of *src_xsec_per_species* with respect to retrieval
quantities.

The variable gives the derivative of *src_xsec_per_species* with
respect to some variables (but not all jacobian variables). Handled
are only variables that are involved in xsec and cannot be
calculated at transmission level

Usage:      Output of *abs_xsec_agenda*.

Dimensions:
     [*abs_species*][n_quantities][*f_grid*, *abs_p* ]
*/
ArrayOfArrayOfMatrix& dsrc_xsec_per_species_dx(Workspace& ws) noexcept {
  return *static_cast<ArrayOfArrayOfMatrix*>(ws[71]);
}

/*! The derivative of *surface_emission* with respect to quantities
listed in *dsurface_names*.

Usage: Used internally of radiative transfer methods

Dimensions: [dsurface_names][f_grid, stokes_dim]
*/
ArrayOfMatrix& dsurface_emission_dx(Workspace& ws) noexcept {
  return *static_cast<ArrayOfMatrix*>(ws[91]);
}

/*! Name of surface retrieval quantities.

Usage: Used internally of radiative transfer methods

Dimensions: [retrieval quantity]
*/
ArrayOfString& dsurface_names(Workspace& ws) noexcept {
  return *static_cast<ArrayOfString*>(ws[92]);
}

/*! The derivative of *surface_rmatrix* with respect to quantities
listed in *dsurface_names*.

Usage: Used internally of radiative transfer methods

Dimensions: [dsurface_names][surface_los, f_grid, stokes_dim, stokes_dim]
*/
ArrayOfTensor4& dsurface_rmatrix_dx(Workspace& ws) noexcept {
  return *static_cast<ArrayOfTensor4*>(ws[93]);
}

/*! Contribution function (or gain) matrix.

This matrix is the partial derivative of the retrieved state vector
with respect to the measurement vector (*y*).

Usage: Used and set by inversion methods.
*/
Matrix& dxdy(Workspace& ws) noexcept { return *static_cast<Matrix*>(ws[94]); }

/*! Total extinction matrix.

This variable contains the extinction coefficient matrix, which
is used in the RT calculation in the cloudbox. It is the physical
extinction matrix which includes particle extinction for all chosen
scattering species and gaseous extinction for all chosen gaseous species.

See the ARTS user guide (AUG) for further information. Use the index to
find where this variable is discussed. The variable is listed as a
subentry to "workspace variables".

Usage:      Output of *opt_prop_bulkCalc*

Unit:       m^2

Dimensions: [f_grid, stokes_dim, stokes_dim]
*/
PropagationMatrix& ext_mat(Workspace& ws) noexcept {
  return *static_cast<PropagationMatrix*>(ws[95]);
}

/*! Extinction matrix for all individual scattering elements.

This variable contains the elements of the extinction matrix of all
individual scattering elements for a given propagation direction. It is
calculated input as well as the output of the agenda *spt_calc_agenda*.

Usage:      Output of *spt_calc_agenda*

Unit:       m^2

Dimensions: [number of scattering elements, stokes_dim, stokes_dim]
*/
ArrayOfPropagationMatrix& ext_mat_spt(Workspace& ws) noexcept {
  return *static_cast<ArrayOfPropagationMatrix*>(ws[96]);
}

/*! The frequency position of each backend (spectrometer) channel.

Usage: Set by the user.

Unit:  Hz
*/
Vector& f_backend(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[102]);
}

/*! As *f_backend* but describes an instrument with muliple
mixer/receiver chains.

This variable is needed when e.g. the receiver has several mixers
or the the receiver measures several polarisation and the channels
differ in position or response function.

The array has one element for each "receiver chain". The array
length must match *backend_channel_response_multi*, and possibly
also *lo_multi*.

Usage: Set by the user.

Unit:  Hz
*/
ArrayOfVector& f_backend_multi(Workspace& ws) noexcept {
  return *static_cast<ArrayOfVector*>(ws[103]);
}

/*! The frequency grid for monochromatic pencil beam calculations.

Usage: Set by the user.

Unit:  Hz
*/
Vector& f_grid(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[104]);
}

/*! Frequency index.

Not all methods handle all monochromatic frequencies (of *f_grid*) in
parellel and this variable is used for communication between methods,
holding the index of the frequency treated presently.

In some contexts, a negative f_index means all frequencies.

Usage: Method output.
*/
Index& f_index(Workspace& ws) noexcept { return *static_cast<Index*>(ws[105]); }

/*! Index number for files.

See *WriteXMLIndexed* for further information.

Usage:   Input to *WriteXMLIndexed* and *ReadXMLIndexed*.
*/
Index& file_index(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[97]);
}

/*! Agenda performing a for loop.
 */
Agenda& forloop_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[98]);
}

/*! The index for for-loops.

This is the index that is used by method *ForLoop* to loop over
*forloop_agenda*.
*/
Index& forloop_index(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[99]);
}

/*! So far just testing of FOS ...
 */
Vector& fos_iyin_za_angles(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[100]);
}

/*! So far just testing of FOS ...
 */
Matrix& fos_scatint_angles(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[101]);
}

/*! Gravity at zero altitude.

This variable is "little g" at the reference ellipsiod. That is,
for Earth this is a value around 9.81 m/s2
*/
Numeric& g0(Workspace& ws) noexcept { return *static_cast<Numeric*>(ws[107]); }

/*! Agenda providing the gravity constant.
 */
Agenda& g0_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[108]);
}

/*! Geo-position of a measurement.

An empty vector is allowed, then flagging that no geo-positioning
has been performed.

Otherwise, this should be a vector having length 5. The elements are:
   altitude
   latitude
   longitide
   zenith angle
   azimuth angle

Dimensions: 0 or 5

Unit:  [ m, deg, deg, deg, deg ]
*/
Vector& geo_pos(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[106]);
}

/*! Agenda deriving the geo-position of a pencil beam calculation.
 */
Agenda& geo_pos_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[109]);
}

/*! The heating rates of atmospheric layers.

The heating rate is defined as the rate of temperature change of an
atmospheric layer due the heating by absorption of radiation or if it
is negative the loss of energy by emission of radiation.

Units: K s^-1

Size: [(cloudbox_limits[1] - cloudbox_limits[0]) +1,
       (cloudbox_limits[3] - cloudbox_limits[2]) +1,
       (cloudbox_limits[5] - cloudbox_limits[4]) +1,
*/
Tensor3& heating_rates(Workspace& ws) noexcept {
  return *static_cast<Tensor3*>(ws[110]);
}

/*! Data for Hitran cross section species.
 */
ArrayOfXsecRecord& hitran_xsec_data(Workspace& ws) noexcept {
  return *static_cast<ArrayOfXsecRecord*>(ws[111]);
}

/*! Definition of the polarisation of an instrument.

The default for output is to give data for the selected Stokes
elements (1:stokes_dim). This variable defines the polarisations
that are actually measured, or are transmitted.

The polarisation states/components are coded as
   0 = Undefined.
   1 = I, total intensity.
   2 = Q, second Stokes component, Iv - Ih.
   3 = U, third Stokes component, I+45 - I-45.
   4 = V, forth Stokes component, Irc - Ilc
   5 = Iv, intensity of vertically polarised component.
   6 = Ih, intensity of horizontally polarised component.
   7 = I+45, intensity of +45 deg linearly polarised component.
   8 = I-45, intensity of -45 deg linearly polarised component.
   9 = Ilhc, intensity of left-hand circularly polarised component.
  10 = Irhc, intensity of right-hand circularly polarised component.

See the documentation for definition of the Stokes vector and the
different components.

If the instrument measures, or transmits, vertical and horizontal
components, this variable shall accordingly be set to [5,6].

Conversion to Planck-BT of components 2-4 requires that component
1 is kept, and must be included as first element.

The shift from the Stokes vector can be made at any stage when of the
sensor response set-up. The responses used must of course be adopted
correspondingly. Or reversed, if the antenna response is defined for
Iv or Ih it could be useful to shift polarisation as first sensor
operation.

Usage: Set by the user.
*/
ArrayOfIndex& instrument_pol(Workspace& ws) noexcept {
  return *static_cast<ArrayOfIndex*>(ws[112]);
}

/*! Multiple definition of instrument polarisation.

Defined as *instrument_pol* but used when multiple polarisations
are possible/required.

Usage: Set by the user.
*/
ArrayOfArrayOfIndex& instrument_pol_array(Workspace& ws) noexcept {
  return *static_cast<ArrayOfArrayOfIndex*>(ws[113]);
}

/*! Agenda recalculating spectra and Jacobian for iterative inversion methods.
 */
Agenda& inversion_iterate_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[115]);
}

/*! Iteration counter variable for *inversion_iterate_agenda*.
 */
Index& inversion_iteration_counter(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[114]);
}

/*! Irradiance also known as flux density.

Radiant flux received by a surface per unit area
seperately for each hemisphere.
The last dimension denotes the hemispheres. The first component is the downward
irradiance and the second component is the upward irradianceUnits: W m^-2

Size: [(cloudbox_limits[1] - cloudbox_limits[0]) +1,
       (cloudbox_limits[3] - cloudbox_limits[2]) +1,
       (cloudbox_limits[5] - cloudbox_limits[4]) +1,
        2 ]
*/
Tensor4& irradiance_field(Workspace& ws) noexcept {
  return *static_cast<Tensor4*>(ws[116]);
}

/*! Contains the isotopologue ratios.

This variable can be set to default values by
calling *isotopologue_ratiosInitFromBuiltin*
*/
SpeciesAuxData& isotopologue_ratios(Workspace& ws) noexcept {
  return *static_cast<SpeciesAuxData*>(ws[117]);
}

/*! Monochromatic pencil beam radiance spectrum.

This variable holds a single spectrum, with values corresponding
to infinite frequency and spatial resolution (compare to *y*).

The variable is used to represent spectra at all positions of the
propagation path and can e.g. temporarily hold radiation entering
the atmosphere from space.

Usage:      Used by radiative transfer methods.

Unit:       For passive observations, as  selected by *iy_unit*.
            For transmission type, same as for transmitted signal.

Dimensions: [ f_grid, stokes_dim ]
*/
Matrix& iy(Workspace& ws) noexcept { return *static_cast<Matrix*>(ws[118]); }

/*! Flag to handle recursive calls of *iy_main_agenda*

The agenda *iy_main_agenda* can be used recursively and this flag
is used to tell the methods inside the agenda which is the primary
 call. This is handled automatically for methods using
*iy_main_agenda*, such as *yCalc*, but the user must set this
variable to 1 if the agenda is called directly inside the control
file (which should be a rare case).
*/
Index& iy_agenda_call1(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[120]);
}

/*! Data auxiliary to *iy*.

Different data beside the direct result of the radiative transfer
calculations (*iy*) can be obtained by this variable. These auxilary
data are selected by *iy_aux_vars*.

Usage:      Provided by some radiative transfer methods.

Dimensions: [quantity][ f_grid, stokes_dim ]
*/
ArrayOfMatrix& iy_aux(Workspace& ws) noexcept {
  return *static_cast<ArrayOfMatrix*>(ws[121]);
}

/*! Selection of quantities for *iy_aux* and when applicable also *y_aux*.

Each element of this string array determines the quantity for the
corresponding element in *iy_aux* and *y_aux* (i.e. the quantities
are stored in the order given in *iy_aux_vars*).

The possible choices vary between the methods. See the WSM you select
for *iy_main_agenda* for the complete set of choices. Please not that
if the calculations are done through *yCalc*, you can not select
along-the-path variables.
*/
ArrayOfString& iy_aux_vars(Workspace& ws) noexcept {
  return *static_cast<ArrayOfString*>(ws[122]);
}

/*! Agenda deriving the intensity at boundary or interior of the cloudbox.
 */
Agenda& iy_cloudbox_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[123]);
}

/*! Identification number of *iy*.

This variable is intended to be an identification number for individual
calculations of *iy*. This id-number can e.g. be used as input to
*WriteXMLIndexed*, to link filenames to the different calculations.

Some methods sets and updates *iy_id*. The general numbering scheme is:
   xxxyyycba
where xxx identifies the row in sensorPos/los (i.e. the mblock_index),
yyy identifies pencil beam direction inside measurement block (should
in general match a row in mblock_dlos_grid), and cba identies later legs
of total propagation paths, where a, b and c identifies secondary, tertiary
and quaternary part, respectively. 1-based numbering is used. That is,
the primary path of the first pencil beam of the first measurement block
has iy_id = 001001000.

Accordingly, the primary propagation path has cba = 000. If the primary path
intersects with the surface, and the downwelling radiation is calculated
for three directions, these secondary paths get cba = 001, 002 and 003.
If tertiary paths appear, they have numbers such as 011.

As the numbering scheme has nine positions, it is suitable to store
files as: WriteXMLIndexed(output_file_format,iy_id,in,filename,9)

Setting of *iy_id* is not yet supported together with scattering
calculations. The value of iy_id then differs, it is either set to 0
or keeps its value set by *yCalc*.
*/
Index& iy_id(Workspace& ws) noexcept { return *static_cast<Index*>(ws[125]); }

/*! Agenda dedicated to *iyIndependentBeamApproximation*.*/
Agenda& iy_independent_beam_approx_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[124]);
}

/*! Agenda dedicated to *iyLoopFrequencies*.*/
Agenda& iy_loop_freqs_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[126]);
}

/*! Agenda calculating the single monochromatic pencil beam spectrum.
 */
Agenda& iy_main_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[127]);
}

/*! Agenda providing the downwelling radiation at the top of the atmosphere.
 */
Agenda& iy_space_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[128]);
}

/*! Agenda providing the upwelling radiation from the surface.
 */
Agenda& iy_surface_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[129]);
}

/*! Upwelling radiation from the surface, divided into surface types.
 */
ArrayOfAgenda& iy_surface_agenda_array(Workspace& ws) noexcept {
  return *static_cast<ArrayOfAgenda*>(ws[130]);
}

/*! Transmission to be included in *iy*.

The calculation of *iy* can be performed over several propation path
branches, and there can be recursive calls of *iy_main_agenda*.
This variable gives the transmission from the end point of the present
branch and the sensor for such recursive cases.

This variable is used purely internally. The exact usage can vary
between different RT integration schemes.

Usage:      Internally inside iy_main_agenda.

Unit:       1

Dimensions: [ f_grid, stokes_dim, stokes_dim ]
*/
Tensor3& iy_transmission(Workspace& ws) noexcept {
  return *static_cast<Tensor3*>(ws[131]);
}

/*! Agenda providing a transmitter signal.
 */
Agenda& iy_transmitter_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[132]);
}

/*! Selection of output unit for some radiative transfer methods.

This variable allows that the unit of the output radiance/intensity
is changed. The possible choices differ between the radiative
methods, including not considering the variable at all.
Accordingly, for details see the radiative method you have selected
(e.g., *iyEmissionStandard*, *iyMC*, *iyActiveSingleScat* and the like).
*/
String& iy_unit(Workspace& ws) noexcept {
  return *static_cast<String*>(ws[133]);
}

/*! Monochromatic pencil beam data for one measurement block.

The data for all *iy* of a measurement block appended to a vector,
following the sorting order used for *y*.

Usage:      Used internally.

Unit:       W / (m^2 Hz sr) or transmission.

Dimensions: [ nlos * nf * stokes_dim ] where nlos is number of rows in
            mblock_dlos_grid, and nf is length of f_grid.
*/
Vector& iyb(Workspace& ws) noexcept { return *static_cast<Vector*>(ws[119]); }

/*! The Jacobian matrix.

The matrix holding the Jacobians of the retrieval quantities. The
matrix has to be initialised before the retrieval quantities can be
defined. Initialisation is done by*jacobianInit*. Retrieval quantities
are then added with *jacobianAdd...* or *retrievalAdd..* methods.

The order between rows and columns follows how data are stored in *y*
and *x*, respectively.

Units:   See the different retrieval quantities.

Dimension: [ y, number of retrieval quantities and grids ]
*/
Matrix& jacobian(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[134]);
}

/*! Pure numerical Jacobian calculation agenda.
 */
Agenda& jacobian_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[135]);
}

/*! Flag to activate (clear-sky) Jacobian calculations.

If this variable is set to 0, no Jacobian calculations will be done,
even if such calculations have been set-up (through the
jacobianAddXxx methods).

Needs to be 0 if cloudy-sky (Doit) Jacobians shall be calculated.
*/
Index& jacobian_do(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[136]);
}

/*! The retrieval quantities in the Jacobian matrix.

An array of retrieval quantities for which the Jacobians are
calculated.

Usage: Quantities are added by the jacobianAdd WSMs.
*/
ArrayOfRetrievalQuantity& jacobian_quantities(Workspace& ws) noexcept {
  return *static_cast<ArrayOfRetrievalQuantity*>(ws[137]);
}

/*! A latitude.

Unit:  degrees
*/
Numeric& lat(Workspace& ws) noexcept { return *static_cast<Numeric*>(ws[138]); }

/*! The latitude grid.

The latitudes for which the atmospheric fields are defined. The
atmosphere is undefined outside the range covered by the grid.
The grid must be sorted in increasing order, with no repetitions.

Geocentric latitudes are used.

For 1D calculations this vector shall be set to be empty.

For 2D cases the latitudes shall be interpreted as the angular
distance inside the orbit plane from the equator (values
outside +-90 deg are allowed).

For 3D, the valid latitude range is [-90,90].

See further the ARTS user guide (AUG). Use the index to find where
this variable is discussed. The variable is listed as a subentry to
"workspace variables".

Usage: Set by the user.

Unit:  degrees
*/
Vector& lat_grid(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[139]);
}

/*! Latitudinal geolocation for 1D and 2D data.

The variables *lat_grid* and *lon_grid* contain true positions only
for 3D. For 1D and 2D, the geographical position is given by
*lat_true* and *lon_true*. Can be left empty when not used.
Otherwise:

   1D: *lat_true* shall have length 1

   2D: Both *lat_true* and *lon_true* shall have a length matching
       *lat_grid*. That is, *lat_true* and *lon_true* shall not be
       seen as grids, they are vectors giving the actual lat or lon
       for each point corresponding to *lat_grid*.

Usage: Set by the user.

Unit:  degrees
*/
Vector& lat_true(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[140]);
}

/*! Flag to check if the line-by-line calculations will work

Usage: Set manually on own risk, or use *lbl_checkedCalc*.

Unit:  Boolean
*/
Index& lbl_checked(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[141]);
}

/*! Irradiance as seen by a single absorption line.

Used internally for, e.g., NLTE effects
*/
Matrix& line_irradiance(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[142]);
}

/*! Transmission as seen by a single absorption line.

Used internally for, e.g., NLTE effects
*/
Tensor3& line_transmission(Workspace& ws) noexcept {
  return *static_cast<Tensor3*>(ws[143]);
}

/*! The series of gamma values for a Marquardt-levenberg inversion.

The values are stored following iteration order, i.e. the first
is the gamma factor for the first iteration etc.
*/
Vector& lm_ga_history(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[177]);
}

/*! The local oscillator frequency.

A local oscillator frequency is used in a heterodyne system when
the mixer folds the spectra from from radio frequencies (RF) to
intermediate frequencies (IF).

Unit:  Hz

Usage: Set by the user.
*/
Numeric& lo(Workspace& ws) noexcept { return *static_cast<Numeric*>(ws[144]); }

/*! Local oscillator frequencies.

As *lo* but describes an instrument with multiple mixers. A vector
element for each LO. The size of this variable and
*sideband_response_multi* shall match, and probably also
*sideband_mode_multi*.

Unit:  Hz

Usage: Set by the user.
*/
Vector& lo_multi(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[145]);
}

/*! A longitude.

Unit:  degrees
*/
Numeric& lon(Workspace& ws) noexcept { return *static_cast<Numeric*>(ws[146]); }

/*! The longitude grid.

The longitudes for which the atmospheric fields are defined. The
atmosphere is undefined outside the range covered by the grid.
The grid must be sorted in increasing order, with no repetitions.

For 1D and 2D, this WSV shall be set to be empty.

Allowed values for longitudes is the range [-360,360]. The difference
between last and first value can not exceed 360 degrees. A difference
of exactly 360 deg. means that the complete globe is covered and no
propagation paths will reach a longitude edge.

See further the ARTS user guide (AUG). Use the index to find where
this variable is discussed. The variable is listed as a subentry to
"workspace variables".

Usage: Set by the user.

Unit:  degrees
*/
Vector& lon_grid(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[147]);
}

/*! Longitudinal geolocation for 1D and 2D data.

The variables *lat_grid* and *lon_grid* contain true positions only
for 3D. For 1D and 2D, the geographical position is given by
*lat_true* and *lon_true*. Can be left empty when not used.
Otherwise:

   1D: *lon_true* shall have length 1

   2D: Both *lat_true* and *lon_true* shall have a length matching
       *lat_grid*. That is, *lat_true* and *lon_true* shall not be
       seen as grids, they are vectors giving the actual lat or lon
       for each point corresponding to *lat_grid*.

Usage: Set by the user.

Unit:  degrees
*/
Vector& lon_true(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[148]);
}

/*! Zonal component of the magnetic field.

The East-West magnetic field component. Positive values, when
pointing eastward.

Can be set to be empty, which is interpreted as zero field strength
everywhere.

Unit:       T

Dimensions: [ p_grid, lat_grid, lon_grid ]  or [ 0 0 0 ].
*/
Tensor3& mag_u_field(Workspace& ws) noexcept {
  return *static_cast<Tensor3*>(ws[149]);
}

/*! Raw zonal component of the magnetic field.

The East-West magnetic field component. Positive values, when
pointing eastward.

Can be set to be empty, which is interpreted as zero field strength
everywhere.

Unit:       T

Dimensions: [ p_grid, lat_grid, lon_grid ].
*/
GriddedField3& mag_u_field_raw(Workspace& ws) noexcept {
  return *static_cast<GriddedField3*>(ws[150]);
}

/*! Meridional component of the magnetic field.

The North-South magnetic field component. Positive values, when
pointing northward.

Can be set to be empty, which is interpreted as zero field strength
everywhere.

Unit:       T

Dimensions: [ p_grid, lat_grid, lon_grid ]  or [ 0 0 0 ].
*/
Tensor3& mag_v_field(Workspace& ws) noexcept {
  return *static_cast<Tensor3*>(ws[151]);
}

/*! Raw meridional component of the magnetic field.

The North-South magnetic field component. Positive values, when
pointing northward.

Can be set to be empty, which is interpreted as zero field strength
everywhere.

Unit:       T

Dimensions: [ p_grid, lat_grid, lon_grid ].
*/
GriddedField3& mag_v_field_raw(Workspace& ws) noexcept {
  return *static_cast<GriddedField3*>(ws[152]);
}

/*! Vertical component of the magnetic field.

Positive values, when pointing upward.

Can be set to be empty, which is interpreted as zero field strength
everywhere.

Unit:       T

Dimensions: [ p_grid, lat_grid, lon_grid ]  or [ 0 0 0 ].
*/
Tensor3& mag_w_field(Workspace& ws) noexcept {
  return *static_cast<Tensor3*>(ws[153]);
}

/*! Raw vertical component of the magnetic field.

Positive values, when pointing upward.

Can be set to be empty, which is interpreted as zero field strength
everywhere.

Unit:       T

Dimensions: [ p_grid, lat_grid, lon_grid ].
*/
GriddedField3& mag_w_field_raw(Workspace& ws) noexcept {
  return *static_cast<GriddedField3*>(ws[154]);
}

/*! Agenda corresponding to the entire controlfile.
 */
Agenda& main_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[155]);
}

/*! The set of angular pencil beam directions for each measurement block.

The relative angles in this variable are angular off-sets with
respect to the angles in *sensor_los*.

The first column holds the relative zenith angle. This column is
mandatory for all atmospheric dimensionalities. For 3D, there can
also be a second column, giving relative azimuth angles. If this
column is not present (for 3D) zero azimuth off-sets are assumed.

This rule applies to all WSVs of dlos-type, while for WSVs holding
absolute angles (los-type, such as *sensor_los*), the second column
is mandatory for 3D.

See further the ARTS user guide (AUG). Use the index to find where
this variable is discussed. The variable is listed as a subentry to
"workspace variables".

Usage: Set by the user or output of antenna WSMs.

Unit:  degrees
*/
Matrix& mblock_dlos_grid(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[156]);
}

/*! Measurement block index.

Used to tell agendas the index of present measurement block.

Usage: Used internally.
*/
Index& mblock_index(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[157]);
}

/*! Antenna pattern description for dedicated MC calculaions.

Usage: Input to MCGeneral. Set by *mc_antennaSetGaussian* and similar
       methods.
*/
MCAntenna& mc_antenna(Workspace& ws) noexcept {
  return *static_cast<MCAntenna*>(ws[158]);
}

/*! Error in simulated *y* when using a Monte Carlo approach.

Usage: Output from Monte Carlo functions.

Units: Depends on *iy_unit*.

Size:  [ stokes_dim ]
*/
Vector& mc_error(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[159]);
}

/*! Counts the number of iterations (or photons) used in the MC
scattering algorithm.

Usage: Set by MCGeneral and other MC methods.
*/
Index& mc_iteration_count(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[160]);
}

/*! The maximum number of iterations allowed for Monte Carlo
calculations.

Usage: Set by the user.
*/
Index& mc_max_iter(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[161]);
}

/*! The maximum scattering order allowed for Monte Carlo
radar calculations.

Usage: Set by the user.
*/
Index& mc_max_scatorder(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[162]);
}

/*! The maximum time allowed for Monte Carlo calculations.

Usage: Set by the user.

Unit: s
*/
Index& mc_max_time(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[163]);
}

/*! The minimum number of iterations allowed for Monte Carlo
calculations.

Usage: Set by the user.
*/
Index& mc_min_iter(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[164]);
}

/*! Source to emission, position.

Counts the number of MC endpoints in each grid cell.

Usage: Set by MCGeneral and other MC methods.
*/
Tensor3& mc_points(Workspace& ws) noexcept {
  return *static_cast<Tensor3*>(ws[165]);
}

/*! Number of atmospheric scattering events between emission point and sensor.

The first element gives the number of cases with zero scattering events,
the second the number of single scattering cases etc.

Scattering orders above what the variable can hold are not stored at all.
The number of such cases can be determined by comparing
*mc_iteration_count* with the sum of the elements in this array.

Usage: Set by MCGeneral and other MC methods.
*/
ArrayOfIndex& mc_scat_order(Workspace& ws) noexcept {
  return *static_cast<ArrayOfIndex*>(ws[166]);
}

/*! The integer seed for the random number generator used by
Monte Carlo methods.

Usage: Set by MCSetSeed.
*/
Index& mc_seed(Workspace& ws) noexcept { return *static_cast<Index*>(ws[168]); }

/*! Rough classification of source to emission.

This is an array of length 4, where the elements in order represent
space, the surface, atmospheric gas and atmospheric particle.
The distinction between the two last elements is if the emission
is associated with *vmr_field* or *pnd_field*.

The values of the array give the number of cases where the emission
source was found to be inside each "domain".

Usage: Set by MCGeneral and other MC methods.
*/
ArrayOfIndex& mc_source_domain(Workspace& ws) noexcept {
  return *static_cast<ArrayOfIndex*>(ws[167]);
}

/*! Target precision (1 std. dev.) for Monte Carlo calculations.

Usage: Set by the user.
*/
Numeric& mc_std_err(Workspace& ws) noexcept {
  return *static_cast<Numeric*>(ws[169]);
}

/*! Defines an upper step length in terms of optical thickness for Monte Carlo
calculations.

Usage: Set by the user.
*/
Numeric& mc_taustep_limit(Workspace& ws) noexcept {
  return *static_cast<Numeric*>(ws[171]);
}

/*! Normalized Stokes vector for transmission (e.g., radar).

The first element (intensity) should have a value of 1.
Usage: Set by user.

Units: Unitless.

Size:  [ stokes_dim ]
*/
Vector& mc_y_tx(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[170]);
}

/*! The AMSU data set.

This is intended as input for the method ybatchMetProfiles. It holds the
latitude, longitude, satellite zenith angle and amsu-b corrected and
uncorrected brightness temperatures.  It also has information about
the particular pixel corresponds to a land or sea point.  This will be
read in the method ybatchMetProfiles and the profiles corresponding to
each latitude and longitude will be read in.

See documentation of WSM *ybatchMetProfiles* for more information.
*/
Matrix& met_amsu_data(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[172]);
}

/*! The antenna beam width for meteorological millimeter instruments.

This Vector must match the number and order of channels in
*met_mm_backend*.

Usage: Set by the user.

Unit:  [ Hz ]

Size:  [ number of channels ]
*/
Vector& met_mm_antenna(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[173]);
}

/*! Backend description for meteorological millimeter sensors with passbands.

This is a compact description of a passband-type sensor, e.g. AMSU-A. The matrix
contains one row for each instrument channel. Each row contains four elements:

  LO position [Hz]
  first offset from the LO [Hz]
  second offset from the LO+offset1 [Hz]
  channel width [Hz]

                          LO
                           |
               offset1     |    offset1
           ----------------+----------------
           |                               |
           |                               |
  offset2  |  offset2             offset2  |  offset2
  ---------+---------             ---------+---------
  |                 |             |                 |
  |                 |             |                 |
#####             #####         #####             #####
width             width         width             width

For a sensor with 1 passband, offset1 and offset2 are zero.
For a sensor with 2 passbands, only offset2 is zero.

Usage: Set by the user.

Unit: All entries in Hz.

Size: [number of channels, 4]
*/
Matrix& met_mm_backend(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[174]);
}

/*! The polarisation for meteorological millimeter instruments.

This array must match the number and order of channels in
*met_mm_backend*.

Possible values:
V: Vertical polarisation
H: Horizontal polarisation
LHC: Left-hand circular polarisation
RHC: Right-hand circular polarisation
AMSU-V: Vertical polarisation dependening on AMSU zenith angle
AMSU-H: Horizontal polarisation dependening on AMSU zenith angle
ISMAR-V: Vertical polarisation dependening on ISMAR zenith angle
ISMAR-H: Horizontal polarisation dependening on AMSU zenith angle

Usage: Set by the user.

Unit:  [ String ]

Size:  [ number of channels ]
*/
ArrayOfString& met_mm_polarisation(Workspace& ws) noexcept {
  return *static_cast<ArrayOfString*>(ws[175]);
}

/*! Agenda for metoffice profile calculations.
 */
Agenda& met_profile_calc_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[176]);
}

/*! The average molar mass of dry air.

This could also be referred to as the average molecular weight for
dry air. The definition of "dry air" can differ between planets and
methods using the WSV. For Earth, this should be a value around
28.97.
*/
Numeric& molarmass_dry_air(Workspace& ws) noexcept {
  return *static_cast<Numeric*>(ws[178]);
}

/*! Number of elements in 4th lowest dimension of a Tensor.
 */
Index& nbooks(Workspace& ws) noexcept { return *static_cast<Index*>(ws[187]); }

/*! Number of columns (elements in lowest dimension) of a Matrix or Tensor.
 */
Index& ncols(Workspace& ws) noexcept { return *static_cast<Index*>(ws[184]); }

/*! Number of elements of a Vector or Array.
 */
Index& nelem(Workspace& ws) noexcept { return *static_cast<Index*>(ws[183]); }

/*! Number of elements in 7th lowest dimension of a Tensor.
 */
Index& nlibraries(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[190]);
}

/*! Flag to perform Non-LTE calculations.
 */
Index& nlte_do(Workspace& ws) noexcept { return *static_cast<Index*>(ws[191]); }

/*! NLTE partial derivatives output is two parts:  S*dB/dx+dS/dx*B.
This should contain the first term for one point in the atmosphere
(one set of pressure, temperature, zn magnetic field, and VMR values)
with respect to one of of the input parameters.

Dimensions: [ quantities ] [nza, naa, nf, stokes_dim] or [0]

Unit: 1/m/jacobian_quantity
*/
ArrayOfStokesVector& nlte_dsource_dx(Workspace& ws) noexcept {
  return *static_cast<ArrayOfStokesVector*>(ws[79]);
}

/*! The field of NLTE temperatures and/or ratios.

This variable gives the NLTE temperature/ratio at each crossing of
the pressure, latitude and longitude grids.  The size of the
array is the number of NLTE levels in all molecules.

The temperature/ratio for a point between the grid crossings is obtained
by (multi-)linear interpolation of the *nlte_field*.

There are two types of NLTE computations available in ARTS.  One from
giving excitiation temperatures that makes the absorption/emission diverge
from LTE.  The other is to use the absolute ratios of upper-to-lower states at
the levels of interest.

Units:       [ K or % ]]

Dimensions: [ NLTE levels, p_grid, lat_grid, lon_grid ] or [ 0, 0, 0, 0 ]
*/
EnergyLevelMap& nlte_field(Workspace& ws) noexcept {
  return *static_cast<EnergyLevelMap*>(ws[339]);
}

/*! Raw data for NLTE temperatures and/or ratios.

This variable gives the NLTE temperature/ratio as stored in the
database for the atmospheric scenarios.

See further the ARTS user guide (AUG). Use the index to find where
this variable is discussed. The variable is listed as a subentry to
"workspace variables".

Usage: Set by the user by choosing a climatology.

Unit:  K

Size   ArrayOfGriddedField3
 array:
       [NLTE levels] or [ 0 ]
fields:
       [N_p]
       [N_lat]
       [N_lon]
       [N_p, N_lat, N_lon]
*/
ArrayOfGriddedField3& nlte_field_raw(Workspace& ws) noexcept {
  return *static_cast<ArrayOfGriddedField3*>(ws[341]);
}

/*! An array of non-lte quantum identifiers for levels matching
 *nlte_field_raw* and on request *nlte_vibrational_energies*.
 */
ArrayOfQuantumIdentifier& nlte_level_identifiers(Workspace& ws) noexcept {
  return *static_cast<ArrayOfQuantumIdentifier*>(ws[179]);
}

/*! Variable to contain the additional source function due to NLTE effects.

Dimensions: [ nspecies ] [nza, naa, nf, stokes_dim] or [0]
*/
ArrayOfStokesVector& nlte_source(Workspace& ws) noexcept {
  return *static_cast<ArrayOfStokesVector*>(ws[192]);
}

/*! Analog to *propmat_clearsky_field* for *propmat_clearsky*, but for.
the *nlte_source* variable.

Unit:       1/m

Dimensions: [species, f_grid, *stokes_dim*, p_grid, lat_grid, lon_grid]
*/
Tensor6& nlte_source_field(Workspace& ws) noexcept {
  return *static_cast<Tensor6*>(ws[193]);
}

/*! An list of vibrational energies matching
 *nlte_level_identifiers* and *nlte_field_raw* or being 0.
 */
Vector& nlte_vibrational_energies(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[180]);
}

/*! Number of elements in 3rd lowest dimension of a Tensor.
 */
Index& npages(Workspace& ws) noexcept { return *static_cast<Index*>(ws[186]); }

/*! Number of rows (elements in 2nd lowest dimension) of a Matrix or Tensor.
 */
Index& nrows(Workspace& ws) noexcept { return *static_cast<Index*>(ws[185]); }

/*! Number of elements in 5th lowest dimension of a Tensor.
 */
Index& nshelves(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[188]);
}

/*! Number of elements in 6th lowest dimension of a Tensor.
 */
Index& nvitrines(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[189]);
}

/*! Basic diagnostics of an OEM type inversion.

This is a vector of length 5, having the elements (0-based index):
  0: Convergence status, with coding
       0 = converged
       1 = max iterations reached
       2 = max gamma of LM reached
       9 = some error when calling *inversion_iterate_agenda*
      99 = too high start cost.
  1: Start value of cost function.
  2: End value of cost function.
  3: End value of y-part of cost function.
  4: Number of iterations used.

See WSM *OEM* for a definition of "cost". Values not calculated
are set to NaN.
*/
Vector& oem_diagnostics(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[194]);
}

/*! Errors encountered during OEM execution.
 */
ArrayOfString& oem_errors(Workspace& ws) noexcept {
  return *static_cast<ArrayOfString*>(ws[195]);
}

/*! Output file format.

This variable sets the format for output files. It could be set to
"ascii" for plain xml files, "zascii" for zipped xml files, or
"binary".

To change the value of this variable use the workspace methods
*output_file_formatSetAscii*, *output_file_formatSetZippedAscii*, and
*output_file_formatSetBinary*
*/
String& output_file_format(Workspace& ws) noexcept {
  return *static_cast<String*>(ws[196]);
}

/*! The pressure grid.

The pressure levels on which the atmospheric fields are defined.
This variable must always be defined. The grid must be sorted in
decreasing order, with no repetitions.

No gap between the lowermost pressure level and the surface is
allowed. The uppermost pressure level defines the practical upper
limit of the atmosphere as vacuum is assumed above.

See further the ARTS user guide (AUG). Use the index to find where
this variable is discussed. The variable is listed as a subentry to
"workspace variables".

Usage: Set by the user.

Unit:  Pa
*/
Vector& p_grid(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[242]);
}

/*! The original pressure grid before optimization.

This variable is used to interpolate *cloudbox_field* back to its original
size after the calculation with *OptimizeDoitPressureGrid*.
 The variable is listed as a subentry to
"workspace variables".

Usage: Set by the user.

Unit:  Pa
*/
Vector& p_grid_orig(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[243]);
}

/*! Reference pressure calculation of hydrostatic equilibrium.

The altitude specified by this pressure is used as the reference
when calculating hydrostatic equilibrium. That is, the geometrical
altitude at this pressure is not changed.

Usage: Set by the user.

Unit:  Pa
*/
Numeric& p_hse(Workspace& ws) noexcept {
  return *static_cast<Numeric*>(ws[244]);
}

/*! Container for various data that describes scattering bulk properties.

The number and order of bulk properties is free, as long as the data are
consistent with the content of *particle_bulkprop_names*.

The data shall be given on the standard atmospheric grids. When actually
used, this variable must have zeros at all positions outside and at the
border of the cloudbox.

Dimensions: [ particle_bulkprop_names, p_grid, lat_grid, lon_grid ]
*/
Tensor4& particle_bulkprop_field(Workspace& ws) noexcept {
  return *static_cast<Tensor4*>(ws[197]);
}

/*! Identification of the data in *particle_bulkprop_field*.

This variable assigns a name to each field in *particle_bulkprop_field*.
The naming is totally free. If two fields are given the same name, the
first one will be selected.

Dimensions: length should match book-dimension of *particle_bulkprop_field*
*/
ArrayOfString& particle_bulkprop_names(Workspace& ws) noexcept {
  return *static_cast<ArrayOfString*>(ws[198]);
}

/*! The mass of individual particles (or bulks).

Each row corresponds to a scattering element (i.e. an element in
*scat_data*). The user is free to define different mass
categories and assign a mass for each category. Each column
of *particle_masses* corresponds to such a mass category. A scattering
element can have a non-zero mass for more than one category.

For example, if you work with clouds, your mass categories could
be ice and liquid, corresponding to IWC and LWC, respectively.
The mass of particles inside the melting layer, having a mixed
phase, could be divided between the two columns of the matrix.

Shall either be empty, or have a row size consistent with the
scattering variables (*scat_data*, *pnd_field*).

Usage:      Set by the user.

Unit:       kg

Dimensions: [number of scattering elements, number of mass categories]
*/
Matrix& particle_masses(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[199]);
}

/*! Partition functions.

This variable can be set to default values by
calling *partition_functionsInitFromBuiltin*
*/
SpeciesAuxData& partition_functions(Workspace& ws) noexcept {
  return *static_cast<SpeciesAuxData*>(ws[200]);
}

/*! Ensemble averaged phase matrix.

This workspace variable represents the actual physical phase
matrix (averaged over all scattering elements) for given propagation
directions. It is calculated in the method *pha_matCalc*.

See ARTS user guide (AUG) for further information. Use the index to find
where this variable is discussed. The variable is listed as a subentry
to "workspace variables".

Usage:      Output of the method *pha_matCalc*

Unit:        m^2

Dimensions: [za_grid, aa_grid, stokes_dim, stokes_dim]
*/
Tensor4& pha_mat(Workspace& ws) noexcept {
  return *static_cast<Tensor4*>(ws[201]);
}

/*! Ensemble averaged phase matrix for DOIT calculation.

This workspace variable represents the actual physical phase
matrix (averaged over all scattering elements) for given incident and
propagation directions. It is calculated in the method
*DoitScatteringDataPrepare*.

See ARTS user guide (AUG) for further information.
Usage:      Output of the method *pha_matCalc*

Unit:        m^2

Dimensions: [T,za_grid, aa_grid, za_grid, aa_grid,
 stokes_dim, stokes_dim]
*/
Tensor7& pha_mat_doit(Workspace& ws) noexcept {
  return *static_cast<Tensor7*>(ws[202]);
}

/*! Phase matrix for all individual scattering elements.

This variable contains the elements of phase matrix for all individual
scattering elements for given propagation directions. It is the
calculated in the agenda *pha_mat_spt_agenda*. The elements of the phase
matrix are calculated from the single scattering data.

See ARTS user guide (AUG) for further information.

Usage:      Input and Output of the pha_mat_sptFrom* methods

Unit:       m^2

Dimensions: [number of scattering elements, za_grid, aa_grid,
             stokes_dim, stokes_dim]
*/
Tensor5& pha_mat_spt(Workspace& ws) noexcept {
  return *static_cast<Tensor5*>(ws[203]);
}

/*! Interpolated phase matrix.

This variable contains the data of the phase matrix in the
scattering frame interpolated on the actual frequency (the variable
is used inside *doit_mono_agenda*) and also interpolated on all
possible scattering angles following from all combinations of
*za_grid* and *aa_grid*.

Usage:      Input of the method *pha_mat_sptFromDataDOITOpt*

Unit:        m^2

Dimensions:
[number of scattering elements]
[T, za_grid, aa_grid, za_grid, aa_grid,
 stokes_dim, stokes_dim]
*/
ArrayOfTensor7& pha_mat_sptDOITOpt(Workspace& ws) noexcept {
  return *static_cast<ArrayOfTensor7*>(ws[205]);
}

/*! Agenda calculates the phase matrix for individual scattering elements.
 */
Agenda& pha_mat_spt_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[204]);
}

/*! The sidereal rotation period of the planet.

This is time that it takes for the planet to complete one revolution
around its axis of rotation relative to the stars. For Earth, this
is a value roughly 4 min less than 24 h.

A negative value signifies a retrograde rotation, i.e. opposite to
the rotation of Earth.
Unit:   s
*/
Numeric& planet_rotation_period(Workspace& ws) noexcept {
  return *static_cast<Numeric*>(ws[206]);
}

/*! Mapping of particle bulk properties to number density data.

The length of this agenda array shall match the size of *scat_species*.
That is there is a "pnd-agenda" associated with each scattering species.

In short, each agenda takes some bulk property data as input, and returns
particle number densities for all scattering elements of the species.
See further *pnd_agenda_input* and associated variables.
*/
ArrayOfAgenda& pnd_agenda_array(Workspace& ws) noexcept {
  return *static_cast<ArrayOfAgenda*>(ws[207]);
}

/*! Naming of all input expected by *pnd_agenda_array*.

This variable contains *pnd_agenda_input_names* for each agenda
element in *pnd_agenda_array*.

Dimension: [ n_scattering_species ][ n_input_variables ]
*/
ArrayOfArrayOfString& pnd_agenda_array_input_names(Workspace& ws) noexcept {
  return *static_cast<ArrayOfArrayOfString*>(ws[210]);
}

/*! The variable input to one element of *pnd_agenda_array*.

The column dimension corresponds to the input to the underlying
particle size distribution method. For example, the first column
can hold ice water content values, and the second one temperature
data.

Temperatures are handled by *pnd_agenda_input_t* and shall not be
included in this variable.

Each row corresponds to a position. That is, the methods in the
pnd-agendas are expected to process multiple points in one call.

Dimensions: [ n_points, n_input_variables ]
*/
Matrix& pnd_agenda_input(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[208]);
}

/*! Naming of (existing or expected) data in *pnd_agenda_input*.

The strings of this variable refer to the corresponding column in
*pnd_agenda_input*.

Dimension: [ n_input_variables ]
*/
ArrayOfString& pnd_agenda_input_names(Workspace& ws) noexcept {
  return *static_cast<ArrayOfString*>(ws[211]);
}

/*! Temperature input to one element of *pnd_agenda_array*.

This WSV works as *pnd_agenda_input* but holds a specific quantity,
temperature.

Each element corresponds to a position. That is, the methods in the
pnd-agendas are expected to process multiple points in one call.

Dimensions: [ n_points ]
*/
Vector& pnd_agenda_input_t(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[209]);
}

/*! Particle number density values for a set of points.

The variable contains particle number density data for one scattering
species. The row dimension corresponds to different positions, in the
same way as *pnd_agenda_input* is defined.

Dimensions: [ n_points, n_scattering_elements ]
*/
Matrix& pnd_data(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[212]);
}

/*! Particle number density field.

This variable holds the particle number density fields for all
scattering elements being read in the WSMs
*ScatElementsPndAndScatAdd* or *ScatSpeciesPndAndScatAdd* and
interpolated to the calculation grids *p_grid*, *lat_grid*, and
*lon_grid* inside the cloudbox. An alternative method to create
*pnd_field* is *pnd_fieldCalcFromParticleBulkProps*.

Total number and order of scattering elements in *pnd_field* and (the
flattened) *scat_data* has to be identical.

Note: To ensure that no particles exist outside the cloudbox,
*pnd_field* is required to be 0 at its outer limits (corresponding
to the *cloudbox_limits*).

Usage:      Set by user or output of *pnd_fieldCalcFromParticleBulkProps*

Unit:        m^-3

Size: [number of scattering elements,
       (*cloudbox_limits*[1] - *cloudbox_limits*[0]) +1,
       (*cloudbox_limits*[3] - *cloudbox_limits*[2]) +1,
       (*cloudbox_limits*[5] - *cloudbox_limits*[4]) +1 ]
*/
Tensor4& pnd_field(Workspace& ws) noexcept {
  return *static_cast<Tensor4*>(ws[213]);
}

/*! The particle number density field raw data.

This variable contains the particle number density data for all
considered scattering elements. *pnd_field_raw* is an Array of
GriddedField3. It includes one GriddedField3 for each scattering
element, which contains both the data and the corresponding grids.

Usage: Set by the user. Input to methods *ScatElementsPndAndScatAdd* and
       *ScatSpeciesPndAndScatAdd*

Unit:  m^-3

Size:  Array[number of scattering elementst]
       GriddedField3
        [number of pressure levels]
       [number of latitudes]
       [number of longitudes]
       [number of pressure levels, number of latitudes, number of longitudes]
*/
ArrayOfGriddedField3& pnd_field_raw(Workspace& ws) noexcept {
  return *static_cast<ArrayOfGriddedField3*>(ws[215]);
}

/*! The particle sizes associated with *pnd_data*.

This variable holds the size of each scattering element considered.
Size can be defined differently, depending on particle size distribution
used. Most common choices should by equivalent diameter, maximum diameter
and mass.

Dimension: [ n_sizes ]
*/
Vector& pnd_size_grid(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[214]);
}

/*! The propagation path for one line-of-sight.

This variable describes the total (pencil beam) propagation path for
a given combination of starting point and line-of-sight. The path is
described by a data structure of type Ppath. This structure contains
also additional fields to faciliate the calculation of spectra and
interpolation of the atmospheric fields.

The data struture is too extensive to be described here, but it is
described carefully in the ARTS user guide (AUG). Use the index to
find where the data structure, Ppath, for propagation paths is
discussed. It is listed as a subentry to "data structures".

Usage: Output from *ppath_agenda*.
*/
Ppath& ppath(Workspace& ws) noexcept { return *static_cast<Ppath*>(ws[216]); }

/*! Agenda calculating complete propagation paths.
 */
Agenda& ppath_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[217]);
}

/*! An array meant to build up the necessary geometries for radiative
field calculations.

Can be ordered or not

Size: user-defined
*/
ArrayOfPpath& ppath_field(Workspace& ws) noexcept {
  return *static_cast<ArrayOfPpath*>(ws[218]);
}

/*! Flag to perform ray tracing inside the cloudbox.

Standard propagation path calculations stop at the boundary of the
cloudbox, or stop directly if started inside the cloudbox. This WSV
allows scattering methods to obtain propagation paths inside the
cloudbox. Hence, this variable is for internal usage primarily.

Usage: For communication between modules of arts.
*/
Index& ppath_inside_cloudbox_do(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[219]);
}

/*! Maximum length between points describing propagation paths.

See *ppath_stepGeometric* for a description of this variable.

Usage: Ppath methods such as *ppath_stepGeometric*.
*/
Numeric& ppath_lmax(Workspace& ws) noexcept {
  return *static_cast<Numeric*>(ws[220]);
}

/*! Maximum length of ray tracing steps when determining propagation
paths.

See *ppath_stepRefractionBasic* for a description of this variable.

Usage: Refraction ppath methods such as *ppath_stepRefractionBasic*.
*/
Numeric& ppath_lraytrace(Workspace& ws) noexcept {
  return *static_cast<Numeric*>(ws[221]);
}

/*! A propagation path step.

The main intention of this variable is communication with the agenda
*ppath_step_agenda*.

See *ppath_step_agenda* for more information on this variable and
the calculation of propagation paths. Or read the chapter on
propagation paths in the ARTS user guide.

Usage:   In/output to/from *ppath_step_agenda*.

Members: See AUG.
*/
Ppath& ppath_step(Workspace& ws) noexcept {
  return *static_cast<Ppath*>(ws[222]);
}

/*! Agenda calculating a propagation path step.
 */
Agenda& ppath_step_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[223]);
}

/*! Doppler adjusted frequencies along the propagation path.

See *ppvar_p* for a general description of WSVs of ppvar-type.

Dimension: [ number of frequencies, ppath.np ]

Usage: Output of radiative transfer methods.
*/
Matrix& ppvar_f(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[224]);
}

/*! iy-values along the propagation path.

See *ppvar_p* for a general description of WSVs of ppvar-type.

Dimension: [ number of frequencies, stokes_dim, ppath.np ]

Usage: Output of radiative transfer methods.
*/
Tensor3& ppvar_iy(Workspace& ws) noexcept {
  return *static_cast<Tensor3*>(ws[225]);
}

/*! Magnetic field along the propagation path.

See *ppvar_p* for a general description of WSVs of ppvar-type.

Dimension: [ 3, ppath.np ]

Usage: Output of radiative transfer methods.
*/
Matrix& ppvar_mag(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[226]);
}

/*! Non-LTE temperatures/ratios along the propagation path.

See *ppvar_p* for a general description of WSVs of ppvar-type.

Dimension: [ number of non-lte temperatures, 1, 1, ppath.np ]

Usage: Output of radiative transfer methods.
*/
EnergyLevelMap& ppvar_nlte(Workspace& ws) noexcept {
  return *static_cast<EnergyLevelMap*>(ws[227]);
}

/*! The optical depth between the sensor and each point of the propagation path.

Returned as the one-way optical depth even in the case of radar
simulations. Just a scalar value, i.e. no polarisation information is
provided.

See *ppvar_p* for a general description of WSVs of ppvar-type.

Dimension: [ ppath.np, f_grid]

Usage: Output of radiative transfer methods.
*/
Matrix& ppvar_optical_depth(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[230]);
}

/*! Pressure along the propagation path.

ppvar stands for propagation path variable. The variables named in is
way describe the atmosphere and its properties at each point of the
propagation path

Dimension: [ ppath.np ]

Usage: Output of radiative transfer methods.
*/
Vector& ppvar_p(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[228]);
}

/*! PND values along the propagation path.

See *ppvar_p* for a general description of WSVs of ppvar-type.

Dimension: [ number of scattering elements, ppath.np ]

Usage: Output of radiative transfer methods.
*/
Matrix& ppvar_pnd(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[229]);
}

/*! Temperature along the propagation path.

See *ppvar_p* for a general description of WSVs of ppvar-type.

Dimension: [ ppath.np ]

Usage: Output of radiative transfer methods.
*/
Vector& ppvar_t(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[231]);
}

/*! The transmission between the sensor and each point of the propagation path.

Returned as the one-way transmission even in the case of radar
simulations.

See *ppvar_p* for a general description of WSVs of ppvar-type.

Dimension: [ ppath.np, f_grid, stokes_dim, stokes_dim ]

Usage: Output of radiative transfer methods.
*/
Tensor4& ppvar_trans_cumulat(Workspace& ws) noexcept {
  return *static_cast<Tensor4*>(ws[232]);
}

/*! The transmission between each point along the propagation path.

See *ppvar_p* for a general description of WSVs of ppvar-type.

Dimension: [ ppath.np, f_grid, stokes_dim, stokes_dim ]

Usage: Output of radiative transfer methods.
*/
Tensor4& ppvar_trans_partial(Workspace& ws) noexcept {
  return *static_cast<Tensor4*>(ws[233]);
}

/*! VMR values along the propagation path.

See *ppvar_p* for a general description of WSVs of ppvar-type.

Dimension: [ number of abs. species, ppath.np ]

Usage: Output of radiative transfer methods.
*/
Matrix& ppvar_vmr(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[234]);
}

/*! Winds along the propagation path.

See *ppvar_p* for a general description of WSVs of ppvar-type.

Dimension: [ 3, ppath.np ]

Usage: Output of radiative transfer methods.
*/
Matrix& ppvar_wind(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[235]);
}

/*! This contains the absorption coefficients for one point in the
atmosphere (one set of pressure, temperature, magnetic field, and
VMR values).

Dimensions: [ abs_species ] [naa, nza, nf, f(stokes_dim)]

Unit: 1/m
*/
ArrayOfPropagationMatrix& propmat_clearsky(Workspace& ws) noexcept {
  return *static_cast<ArrayOfPropagationMatrix*>(ws[236]);
}

/*! Agenda calculating the absorption coefficient matrices.
 */
Agenda& propmat_clearsky_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[238]);
}

/*! OK-flag for *propmat_clearsky_agenda*.

Set by *propmat_clearsky_agenda_checkedCalc*.
*/
Index& propmat_clearsky_agenda_checked(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[237]);
}

/*! Gas absorption field.

Contains the (polarized) gas absorption coefficients for all species
as a function of *f_grid*, *p_grid*, *lat_grid*, and *lon_grid*.

This is mainly for testing and plotting gas absorption. For RT
calculations, gas absorption is calculated or extracted locally,
therefore there is no need to store a global field. But this variable
is handy for easy plotting of absorption vs. pressure, for example.

Unit:       1/m

Dimensions: [species, f_grid, *stokes_dim*, stokes_dim, p_grid, lat_grid,
lon_grid]
*/
Tensor7& propmat_clearsky_field(Workspace& ws) noexcept {
  return *static_cast<Tensor7*>(ws[239]);
}

/*! Particle size distribution values for a set of points.

The variable contains particle size distribution data for one scattering
species. The row dimension corresponds to different positions, in the
same way as *pnd_agenda_input* is defined.

Dimensions: [ n_points, n_scattering_elements ]
*/
Matrix& psd_data(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[240]);
}

/*! The particle sizes associated with *psd_data*.

This variable holds the size of each scattering element considered.
Size can be defined differently, depending on particle size distribution
used. Most common choices should by equivalent diameter, maximum diameter
and mass.

Dimension: [ n_sizes ]
*/
Vector& psd_size_grid(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[241]);
}

/*! Radiant flux per unit solid angle per unit projected area
seperately for each hemisphere.

The last dimension denotes the hemispheres. The first component is the downward
radiance and the second component is the upward radianceUnits: W / (m^2 sr)

Size: [(cloudbox_limits[1] - cloudbox_limits[0]) +1,
       (cloudbox_limits[3] - cloudbox_limits[2]) +1,
       (cloudbox_limits[5] - cloudbox_limits[4]) +1,
        N_za, N_aa
*/
Tensor5& radiance_field(Workspace& ws) noexcept {
  return *static_cast<Tensor5*>(ws[245]);
}

/*! The range bins of an active instrument.

The bins are assumed to cover a range without gaps, and the bins are
defined by their edges. That is, the length of this vector is the
number of bins + 1.

The bins can potentially be defined in two ways, by altitude or time.
See the method you are using, if this variable shall hold time or
altitude (or maybe both options are treated).

Unit: m or s
*/
Vector& range_bins(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[246]);
}

/*! Reference ellipsoid.

This vector specifies the shape of the reference ellipsoid. The
vector must have length 2, where the two elements are:
  1: Equatorial radius.
  2: The eccentricity.
The eccentricity is sqrt(1-b*b/a*a) where a and b are equatorial and
polar radius, respectively. If the eccentricity is set to 0, an
average radius should be used instead of the equatorial one.

The eccentricity must be 0 for 1D calculations, as a spherical Earth
is implied by setting *atmosphere_dim* to 1. For 2D, the selected
ellipsoid parameters should be selected according to cross-section
between the real ellipsoid and the 2D plane considered. That is
the applied ellipsoid shall have een converted to match the internal
treatment of 2D cases. For 3D, models can be used, such as WGS84.

Usage:  Set by the user.

Size:   [ 2 ]
*/
Vector& refellipsoid(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[250]);
}

/*! Real part of the refractive index of air.

The variable contains the refractive index summed over all relevant
constituents, at one position in the atmosphere. This refractive
is related to the phase velocity. See also *refr_index_air_group*.

Unit: 1
*/
Numeric& refr_index_air(Workspace& ws) noexcept {
  return *static_cast<Numeric*>(ws[247]);
}

/*! Agenda calculating the refractive index of air.
 */
Agenda& refr_index_air_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[248]);
}

/*! Group index of refractivity.

This variable is defined as the ratio between group velocity and the
speed of ligh in vacuum. That is, it is defined as the "standard"
refractive index, but refers to the group velocity instead of the
phase velocity. See also *refr_index_air*.

Unit: 1
*/
Numeric& refr_index_air_group(Workspace& ws) noexcept {
  return *static_cast<Numeric*>(ws[249]);
}

/*! Relaxation matrix per band per pressure level.

Dimensions: [pressures][band][n_linex, nlines]
Units: Hz/Pa in HWHM
*/
ArrayOfArrayOfMatrix& relmat_per_band(Workspace& ws) noexcept {
  return *static_cast<ArrayOfArrayOfMatrix*>(ws[251]);
}

/*! Used to set the type of line mixing relaxation matrix
that will be calculated.

Supported types by index:
   0: Hartmann-Tran type relaxation matrix.
   1: Linear type relaxation matrix.

Dimensions: [number of bands]
*/
ArrayOfIndex& relmat_type_per_band(Workspace& ws) noexcept {
  return *static_cast<ArrayOfIndex*>(ws[252]);
}

/*! Flag indicating completeness and consistency of retrieval setup.

Unit: Boolean
*/
Index& retrieval_checked(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[253]);
}

/*! The estimated error in the retrieval due to uncertainty in the observations.

The vector contains the square roots  of the diagonal elements of  the
covariance matrix of the error due to measurement noise, S_m in Rodgers'
 book.
*/
Vector& retrieval_eo(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[254]);
}

/*! The estimated error in the retrieval due to limited resolution of the
observation system.

The vector contains the square roots of the diagonal
elements of the covariance matrix of the smoothing error, S_s in Rodgers'
book.
*/
Vector& retrieval_ss(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[255]);
}

/*! Velocity along the line-of-sight to consider for a RT calculation.

This variable gives the velocity of the imaginary detector in
monochromatic pencil beam calculations. The relevant velocity is
the projection along the line-of-sight (ie. total velocity shall not
be given). A positive value means a movement of the detector in the
same direction as the line-of-sight.

This variable is required to include Doppler effects due to
velocities of the observer, relative the centre of the coordinate
system used that is fixed to the planets centre point.

Unit: [ m/s ]
*/
Numeric& rte_alonglos_v(Workspace& ws) noexcept {
  return *static_cast<Numeric*>(ws[256]);
}

/*! A line-of-sight for (complete) radiative transfer calculations.

This variable gives the observation direction for monochromatic
pencil beam calculations. Hence, it is the line-of-sight at the end
point of the propagation path.

For 1D and 2D cases, *rte_los* is a vector of length 1 holding the
zenith angle. For 3D, the length of the vector is 2, where the
additional element is the azimuthal angle. These angles are defined
in the ARTS user guide (AUG). Look in the index for "zenith angle"
and "azimuthal angle".

Usage: See above.

Units: [ degree, degree ]

Size:  [ 1 or 2 ]
*/
Vector& rte_los(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[257]);
}

/*! A geographical position for starting radiative transfer calculations.

This variable gives the observation position for monochromatic
pencil beam calculations. Hence, it is the end point of the
propagation path.

This variable is a vector with a length equalling the atmospheric
dimensionality. The first element is the geometrical altitude.
Element 2 is the latitude and element 3 is the longitude.

Usage: See above.

Units: [ m, degree, degree ]

Size:  [ atmosphere_dim ]
*/
Vector& rte_pos(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[258]);
}

/*! A second geographical position to define the geometry for
radiative transfer calculations.

This variable is used when the propagation path is defined by two
positions, instead of a position (*rte_pos*) and a line-of-sight
(*rte_los*). That is, this variable basically replaces *rte_los*
for the cases of consideration. In practice, *rte_los* is determined
by finding the propagation path between *rte_pos* and *rte_pos2*.

As *rte_pos* with the exception that a "latitude" must also be
specified for 1D. This is the angular distance to *rte_pos*, where
this distance is defined as the 2D-"latitude".

Usage: See above.

Units: [ m, degree, degree ]

Size:  [ atmosphere_dim ]
*/
Vector& rte_pos2(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[259]);
}

/*! Line-of-sight at a radiative transfer point.

This variable holds a local line-of-sight. The angles of this
vector are defined as for *rte_los*.

The WSV is used as input to methods and agendas calculating radiative
properties for a given conditions.

Usage: Communication variable.

Units: [ degree, degree ]

Size:  [ 1 or 2 ]
*/
Vector& rtp_los(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[261]);
}

/*! Magnetic field at a radiative transfer point.

See *mag_u_field* etc. for a definition of the different components.
For this variable the components are put together and thus defines
magnetic field vector. Hence, this is a vector of length three, even
if any of the input fields is set to be empty.

The WSV is used as input to methods and agendas calculating radiative
properties for a given conditions.

Usage: Communication variable.

Units: T

Size:  [ u-component, v-component, w-component ]
*/
Vector& rtp_mag(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[260]);
}

/*! NLTE temperature/ratio at a radiative transfer point.

This vector variable can hold the NLTE temperature/ratio. It is intended
mainly for communication with various methods and agendas, such as
methods and agendas calculating absorption coefficients.
The WSV is used as input to methods and agendas calculating radiative
properties for a given conditions.

Usage: Communication variable.

Units: [ K/# ]
Size:  [ NLTE levels, 1, 1, 1 ] or [ 0, 0, 0, 0 ]
*/
EnergyLevelMap& rtp_nlte(Workspace& ws) noexcept {
  return *static_cast<EnergyLevelMap*>(ws[265]);
}

/*! Position of a radiative transfer point.

This vector is defined as *rte_pos*, but holds a position along
the propgation path, or the start point for new paths, in contrast
to *rte_pos* that is position of the (imaginary) detector.

The WSV is used as input to methods and agendas calculating radiative
properties for a given conditions.

Usage: Communication variable.

Units: [ m, degree, degree ]

Size:  [ atmosphere_dim ]
*/
Vector& rtp_pos(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[262]);
}

/*! Pressure at a radiative transfer point.

This scalar variable holds the local pressure.

The WSV is used as input to methods and agendas calculating radiative
properties for a given conditions.

Usage: Communication variable.

Units: [ Pa ]
*/
Numeric& rtp_pressure(Workspace& ws) noexcept {
  return *static_cast<Numeric*>(ws[263]);
}

/*! Temperature at a radiative transfer point.

This scalar variable can hold the local temperature. It is intended
mainly for communication with various methods and agendas, such as
methods and agendas calculating absorption coefficients.
The WSV is used as input to methods and agendas calculating radiative
properties for a given conditions.

Usage: Communication variable.

Units: [ K ]
*/
Numeric& rtp_temperature(Workspace& ws) noexcept {
  return *static_cast<Numeric*>(ws[264]);
}

/*! Absorption species abundances for radiative transfer calculations.

This vector variable holds the local abundance of the constituents
included in *abs_species*.

The WSV is used as input to methods and agendas calculating radiative
properties for a given conditions.

Usage: Communication variable.

Units: [ Differ between the elements, can be VMR, kg/m3 or #/m3. ]

Size:  Should match abs_species.nelem()
*/
Vector& rtp_vmr(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[266]);
}

/*! Array of single scattering data.

As *scat_data_raw*, but with frequency grids and dimensions reduced
to the RT's *f_grid* or a single frequency entry. Also, temperature
grid or dimensions can be reduced to a single entry, meaning no
temperature interpolation is done for the respective data.

Standard approach to derive scat_data is to use *scat_dataCalc* to
derive it from *scat_data_raw*.*/
ArrayOfArrayOfSingleScatteringData& scat_data(Workspace& ws) noexcept {
  return *static_cast<ArrayOfArrayOfSingleScatteringData*>(ws[267]);
}

/*! OK-flag for *scat_data*.

Relevant checks are performed by *scat_data_checkedCalc. Only the
value 1 is taken as OK.
*/
Index& scat_data_checked(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[268]);
}

/*! Monochromatic single scattering data.

This variable holds the single scattering properties for all
scattering species and scattering elements for a specified frequency.
It can be calculated from *scat_data* using *scat_data_monoCalc*,
which interpolates *scat_data* to the required frequency.
*/
ArrayOfArrayOfSingleScatteringData& scat_data_mono(Workspace& ws) noexcept {
  return *static_cast<ArrayOfArrayOfSingleScatteringData*>(ws[270]);
}

/*! Array of raw single scattering data.

This variable holds the single scattering properties for all
scattering elements, organized according to their assignment to a
scattering species. *scat_data_raw* entries can be derived from
precalculated data files using the methods *ScatElementsPndAndScatAdd*,
*ScatSpeciesPndAndScatAdd*, or *ScatSpeciesScatAndMetaRead* or
can be calculated using *scat_data_singleTmatrix*.

This may be used in combination with *scat_meta*

Usage: Method ouput.

Members: SingleScatteringData:
  Enum[ptype attribute]
  String[description]
  Vector[f_grid]
  Vector[T_grid]
  Vector[za_grid]
  Vector[aa_grid]
  Tensor7[pha_mat_data]
      [f_grid, T_grid, za_grid, aa_grid, za_grid, aa_grid, matrix_element]
                       ^^^^^^^^^^^^^^^^  ^^^^^^^^^^^^^^^^
                       scattered         incoming
  Tensor5[ext_mat_data]
      [f_grid, T_grid, za_grid, aa_grid, matrix_element]
  Tensor5[abs_vec_data]
      [f_grid, T_grid, za_grid, aa_grid, matrix_element]

Dimensions: [number of scattering species][number of scattering elements]
*/
ArrayOfArrayOfSingleScatteringData& scat_data_raw(Workspace& ws) noexcept {
  return *static_cast<ArrayOfArrayOfSingleScatteringData*>(ws[269]);
}

/*! Structure for the single scattering data.

Comprises the single scattering data of a single scattering element.
See ARTS user guide for further information.

Usage: Set by the user.

Dimensions:  SingleScatteringData
  Enum[ptype attribute]
  String[description]
  Vector[f_grid]
  Vector[T_grid]
  Vector[za_grid]
  Vector[aa_grid]
  Tensor7[pha_mat_data]
      [f_grid, T_grid, za_grid, aa_grid, za_grid, aa_grid, matrix_element]
                       ^^^^^^^^^^^^^^^^  ^^^^^^^^^^^^^^^^
                       scattered         incoming
  Tensor5[ext_mat_data]
      [f_grid, T_grid, za_grid, aa_grid, matrix_element]
  Tensor5[abs_vec_data]
      [f_grid, T_grid, za_grid, aa_grid, matrix_element]
*/
SingleScatteringData& scat_data_single(Workspace& ws) noexcept {
  return *static_cast<SingleScatteringData*>(ws[271]);
}

/*! Latitude index for scattering calculations.

This variable is used in methods used for computing scattering
properties of scattering elements like *opt_prop_sptFromData* and
*pha_matCalc*. It holds the information about the position for which the
scattering calculations are done.

Usage:    Input to the methods *spt_calc_agenda*,
                               *pha_mat_spt_agenda*
*/
Index& scat_lat_index(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[272]);
}

/*! Longitude index for scattering calculations.

This variable is used in methods used for computing scattering
properties of scattering elements like *opt_prop_sptFromData* and
*pha_matCalc*. It holds the information about the position for which the
scattering calculations are done.

Usage:    Input to the methods *spt_calc_agenda*,
                               *pha_mat_spt_agenda*
*/
Index& scat_lon_index(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[273]);
}

/*! An Array of scattering meta data (*scat_meta_single*).

The array holds the meta data for all scattering elements. For a
description of the meta data contents refer to the documentation
of *scat_data_single*.

Corresponding to *scat_data*, it is organized in terms of scattering
species (i.e., one sub-array per scattering species holding one
*scat_meta_single* instance per scattering element assigned to this
scattering species). It is primarily used for particle size and shape
distribution calculations using *pnd_fieldCalcFromParticleBulkProps*.
It is also applied for deducing microphysical characterizations of
scattering species, e.g., by *particle_massesFromMetaData*.

Note: This array must contain as many elements as *scat_data* (on
both array levels).

Usage: Set by the user.

Dimensions: [scattering species][scattering elements]
For more details, see also *scat_meta_single*.
*/
ArrayOfArrayOfScatteringMetaData& scat_meta(Workspace& ws) noexcept {
  return *static_cast<ArrayOfArrayOfScatteringMetaData*>(ws[275]);
}

/*! Structure for the scattering meta data.

This variable holds the scattering meta data for a single scattering
element (see AUG for definition). Scattering meta data comprises
the microphysical description of the scattering element as necessary
to relate single scattering properties with mass density or flux
fields. That is, e.g., in order to handle the scattering element in
particle size (and shape) distribution calculations.

For a definition of the structure members see below.

Members of Numeric type can be flagged as unknown by setting them to
NAN. This will cause a runtime error in case the parameter is needed in
the calculation, but will be ignored otherwise.

Usage: Set by the user.

Members:
  description [*String*]
    Description: Free-form description of the scattering element,
    holding information deemed of interest by the user but not covered
    by other structure members (and not used within ARTS).
  source [*String*]
    Description: Free-form description of the source of the data,
    e.g., Mie, T-Matrix, or DDA calculation or a database or a
    literature source.
  refr_index [*String*]
    Description: Free-form description of the underlying complex
    refractive index data, e.g., a literature source.
  mass [*Numeric*]
    Unit: [kg]
    Description: The mass of the scattering element.
  diameter_max [*Numeric*]
    Unit: [m]
    Description: The maximum diameter (or dimension) of the scattering
    element, defined by the circumferential sphere diameter of the
    element. Note that this parameter is only used by some size
    distributions; it does not have a proper meaning if the scattering
    element represents an ensemble of differently sized particles.
  diameter_volume_equ [*Numeric*]
    Unit: [m]
    Description: The volume equivalent sphere diameter of the
    scattering element, i.e., the diameter of a sphere with the same
    volume. For nonspherical particles, volume refers to the volume
    of the particle-forming substance, not that of the circumferential
    sphere (which can be derived from diameter_max). If the particle
    consists of a mixture of materials, the substance
    encompasses the complete mixture. E.g., the substance of 'soft'
    ice particles includes both the ice and the air.
  diameter_area_equ_aerodynamical [*Numeric*]
    Unit: [m]
    Description: The area equivalent sphere diameter of the
    scattering element, i.e., the diameter of a sphere with the same
    cross-sectional area. Here, area refers to the aerodynamically
    relevant area, i.e., the cross-sectional area perpendicular to the
    direction of fall. Similarly to volume in the definition of
    diameter_volume_equ, for non-spherical and mixed-material
    particles, area refers to the area covered by the substance
    mixture of the particle.
*/
ScatteringMetaData& scat_meta_single(Workspace& ws) noexcept {
  return *static_cast<ScatteringMetaData*>(ws[274]);
}

/*! Pressure index for scattering calculations.

This variable is used in methods used for computing scattering
properties of scattering elements like *opt_prop_sptFromData* and
*pha_matCalc*. It holds the information about the location for which the
scattering calculations are done.

Usage:    Input to the methods *spt_calc_agenda*,
                               *pha_mat_spt_agenda*
*/
Index& scat_p_index(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[276]);
}

/*! Array of Strings defining the scattering species to consider.

Each String contains the information to connect scattering species
(e.g., hydrometeor) atmospheric fields with the microphysical
information like size and shape distributions. The strings follow
the following structure with individual elements separated by dashes:

- scattering species name [*String*]
  the name of the scattering species' atmospheric field. Free form,
  but is matched to *atm_fields_compact* fields by their names.
  Common are, e.g., IWC (ice water content), LWC (liquid water
  content), RR (rain rate), and SR (snow rate).
- particle size distribution [*String*]:
  the size distribution function/parametrization to apply. For
  currently possible PSDs see *pnd_fieldCalcFromParticleBulkProps*.

Example: [''IWC-MH97'', ''LWC-H98_STCO'', ...]
*/
ArrayOfString& scat_species(Workspace& ws) noexcept {
  return *static_cast<ArrayOfString*>(ws[277]);
}

/*! Mass-size relationship parameter, for one scattering species.

Some methods require a relationship between mass and particle size,
valid for the complete scattering species. A common model for this
relationship is:
    mass(x) = a * x^b,
where x is size (that could be Dveq, Dmax or mass) and a/b are parameters.

This WSV is a in the expression above.
The WSV matching b is *scat_species_b*.
The WSV matching x is *scat_species_x*.
*/
Numeric& scat_species_a(Workspace& ws) noexcept {
  return *static_cast<Numeric*>(ws[278]);
}

/*! Mass-size relationship parameter, for one scattering species.

See *scat_species_a* for details.
*/
Numeric& scat_species_b(Workspace& ws) noexcept {
  return *static_cast<Numeric*>(ws[279]);
}

/*! The size grid of one scattering species.

The variable holds the sizes associated with one scattering species.
The typical application of these data are as the size grid when
calculating particle size distributions.

The user must set this WSV as several quantities can be used as size,
such as mass and maximum diamater.

See also *scat_species_a*, for example usage of this WSV.

Dimension:  [number of scattering elements]
*/
Vector& scat_species_x(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[280]);
}

/*! OK-flag for sensor related variables.

This variable flags that sensor variables are defined in a formally
and practically correct way. For example, it checks for correct
dimensions of *sensor_pos* and *sensor_los*.

Shall be set by *sensor_checkedCalc*. See that WSM for treated WSVs.
Only the value 1 is taken as OK.
*/
Index& sensor_checked(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[281]);
}

/*! Sensor description for simple AMSU setup.

This is a compact description of an AMSU-type sensor. The matrix
contains one row for each instrument channel. Each row contains three
elements: LO position [Hz], offset of the channel center from the LO
[Hz], and channel width [Hz].

Usage: Set by the user.

Unit: All entries in Hz.

Size: [number of channels, 3]
*/
Matrix& sensor_description_amsu(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[282]);
}

/*! The sensor line-of-sight (LOS) for each measurement block.

Line-of-sights are specified by giving the zenith and azimuth angles.
Column 1 holds the zenith angle. This angle is simply the angle
between the zenith and LOS directions. For 1D and 3D the valid
range is [0 180], while for 2D angles down to -180 degrees are
allowed. Negative angles signifies for 2D observations towards
lower latitudes, while positive angles means observations towards
higher latitudes. Nadir corresponds throughout to 180 degrees.

The azimuth angle is given with respect to the meridian plane. That
is, the plane going through the north and south poles. The valid
range is [-180,180] where angles are counted clockwise; 0 means
that the viewing or propagation direction is north-wise and +90 means
that the direction of concern goes eastward.

No azimuth angle shall be specified for 1D and 2D. This angle is in
general of no concern for these atmospheric dimensionalities, but
matter in some cases, such as with respect to the Doppler shift due
to winds. For 1D the azimuth angle is then assumed to be 0 deg, i.e.
the sensor is treated to be directed towards North. For 2D, the
implied azimuth is 0 or 180, depending of the zenith angle is positive
or negative.

See further the ARTS user guide (AUG). Use the index to find where
this variable is discussed. The variable is listed as a subentry to
"workspace variables".

Usage: Set by the user.

Unit:  [ degrees, degrees ]

Size:  [ number of measurement blocks, 1 or 2 ]
*/
Matrix& sensor_los(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[283]);
}

/*! Flag if sensor response should be normalised or not (0 or 1).

If the flag is set to 1 each sensor response is normalised (where
applicable). If set to 0 the sensor responses are left as provided.

See further the ARTS user guide (AUG). Use the index to find where
this variable is discussed. The variable is listed as a sub-entry to
"workspace variables".

Usage: Set by the user.
*/
Index& sensor_norm(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[284]);
}

/*! A set of polarisation response angles.

The standard choice to consider the polarisation response of the
reciever is by *instrument_pol*, and this response becomes then part
of *sensor_response*. However, that choice is not possible when the
polartisation response changes between measurement blocks, and this
variable combined with the *yApplySensorPol* offers an alternative for
such situations. This WSV also allows defintion of an arbitrary
polarisation angle.

When applying the polarisation response by *yApplySensorPol*, this
variable complements *sensor_pos* and *sensor_los*. This WSV matrix
is also a matrix, that shall have the same number of rows as the other
two matrices.

The columns of *sensor_pol* corresponds to the channels/frequencies
of the receiver. Each element gives the polarisation angle. A pure
vertical response has the angle 0 deg, and pure horisontal 90 deg.
If all U values (Stokes element 3) are zero, the sign of the angle does,
not matter, and 0 and 180 degrees give the same result. With non-zero
U, the result of e.g. -45 and +45 degrees differ.

Note that a receiver with a linear response is assumed. Circular
polarisation is not affected by any rotation.

Usage: Set by the user.

Unit:  [ degrees ]

Size:  [ number of measurement blocks, number of channels/frequencies ]
*/
Matrix& sensor_pol(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[285]);
}

/*! The sensor position for each measurement block.

The sensor positions are specified as a matrix, where the number of
columns shall be equal to *atmosphere_dim*. Column 1 shall contain
the altitude of the sensor platform, column 2 the latitude and the
last column the longitude. The number of rows corresponds to the
number of measurement blocks.

Valid range for latitudes in 3D is [-90,90], while for 2D any value
is accepted. Accepted range for longitudes are [-360,360].

See further the ARTS user guide (AUG). Use the index to find where
this variable is discussed. The variable is listed as a subentry to
"workspace variables".

Usage: Set by the user.

Unit:  [ m, degrees, degrees ]

Size:  [ number of measurement blocks, atmosphere_dim ]
*/
Matrix& sensor_pos(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[286]);
}

/*! The matrix modelling the total sensor response.

This matrix describes the sensor respons for one measurement block
The response is assumed to be identical for each such block.

The matrix is the product of all the individual sensor response
matrices. Therefore its dimensions are depending on the total sensor
configuration. The *sensor_response* has to initialised by the
*sensor_responseInit* method.

Usage: Output/input to the *sensor_response...* methods.

Units: -

Dimension: See the individual *sensor_response...* method.
*/
Sparse& sensor_response(Workspace& ws) noexcept {
  return *static_cast<Sparse*>(ws[287]);
}

/*! The relative azimuth angles associated with the output of
*sensor_response*.

The variable shall not be set manually, it will be set together with
*sensor_response* by sensor response WSMs.

Usage: Set by sensor response methods.

Unit:  [ degrees ]
*/
Vector& sensor_response_aa(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[288]);
}

/*! Agenda providing the sensor response data for a measurement block.
 */
Agenda& sensor_response_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[289]);
}

/*! The relative zenith and azimuth angles associated with the output of
*sensor_response*.

Definition of angles match *mblock_dlos_grid*. Works otherwise as
*sensor_response_f*.

The variable shall not be set manually, it will be set together with
*sensor_response* by sensor response WSMs.

Usage: Set by sensor response methods.

Unit:  [ degrees ]
*/
Matrix& sensor_response_dlos(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[290]);
}

/*! The zenith and azimuth angles associated with *sensor_response*.

A variable for communication between sensor response WSMs. Matches
initially *mblock_dlos_grid*, but is later adjusted according to the
sensor specifications. Only defined when a common grid exists. Values
are here not repeated as in *sensor_response_dlos*

Usage: Set by sensor response methods.

Unit:  [ degrees ]
*/
Matrix& sensor_response_dlos_grid(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[291]);
}

/*! The frequencies associated with the output of *sensor_response*.

This vector gives the frequency for each element of the measurement
vector produced inside one measurement block. The frequencies of
the total measurement vector, *y*, are obtained by repeating these
frequencies n times, where n is the number of measurement blocks
(e.g. the number of rows in *sensor_pos*).

The variable shall not be set manually, it will be set together with
*sensor_response* by sensor response WSMs.

Usage: Set by sensor response methods.

Unit:  [ Hz ]
*/
Vector& sensor_response_f(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[292]);
}

/*! The frequency grid associated with *sensor_response*.

A variable for communication between sensor response WSMs. Matches
initially *f_grid*, but is later adjusted according to the sensor
specifications. Only defined when a common grid exists. Values are
here not repeated as in *sensor_response_f*

Usage: Set by sensor response methods.

Unit:  [ Hz ]
*/
Vector& sensor_response_f_grid(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[293]);
}

/*! The polarisation states associated with the output of
*sensor_response*.

Works basically as *sensor_response_f*.

See *instrument_pol* for coding of polarisation states.

The variable shall not be set manually, it will be set together with
*sensor_response* by sensor response WSMs.

Usage: Set by sensor response methods.

Unit:  [ - ]
*/
ArrayOfIndex& sensor_response_pol(Workspace& ws) noexcept {
  return *static_cast<ArrayOfIndex*>(ws[294]);
}

/*! The "polarisation grid" associated with *sensor_response*.

A variable for communication between sensor response WSMs. It is
initially 1:stokes_dim, but can later adjusted according to the
sensor specifications. Only defined when a common grid exists.

See *instrument_pol* for coding of polarisation states.

Usage: Set by sensor response methods.

Unit:  [ - ]
*/
ArrayOfIndex& sensor_response_pol_grid(Workspace& ws) noexcept {
  return *static_cast<ArrayOfIndex*>(ws[295]);
}

/*! The time for each measurement block.

This WSV is used when a time must be assigned to the measurements.
No specific time format has (yet) been specified.

Usage: Set by the user.

Unit:  [ arbitrary ]

Size:  [ number of measurement blocks ]
*/
Vector& sensor_time(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[296]);
}

/*! Description of target sideband.

A text string describing which of the two sidebands (of a heterodyne
instrument) that can be seen as "main" band. Possible choices are:
 "lower" : Low frequency sideband shall be considered as target.
 "upper" : High frequency sideband shall be considered as target.

Usage: Set by the user.
*/
String& sideband_mode(Workspace& ws) noexcept {
  return *static_cast<String*>(ws[297]);
}

/*! Description of target sideband for a multiple LO receiver.

As *sideband_mode* but handles an instrument with several LO chains.
See further *lo_multi* and *sideband_response_multi*. This length of
this array must match the size of those WSVs.

Usage: Set by the user.
*/
ArrayOfString& sideband_mode_multi(Workspace& ws) noexcept {
  return *static_cast<ArrayOfString*>(ws[298]);
}

/*! Description of (mixer) sideband response.

This variable describes the response of each sideband of a heterodyne
receiver. The response is given as a GriddedField1, with frequency as the
grid. The actual data describe the sideband filter function at each
frequency grid point. An interpolation is applied to obtain the
response for other frequencies.

The frequency grid should be given in terms of IF, with end points
symmetrically placed around zero. That is, the grid must contain
both negative and positive values. The sideband response (after
summation with *lo*) is not allowed to extend outside the range
for which spectral data exist (normally determined by *f_grid*).

Usage: Set by the user.

Dimensions:
   GriddedField1:
      Vector f_grid[N_f]
      Vector data[N_f]
*/
GriddedField1& sideband_response(Workspace& ws) noexcept {
  return *static_cast<GriddedField1*>(ws[299]);
}

/*! Description of multiple (mixer) sideband responses.

As *sideband_response* but describes an instrument with multiple
mixers. An array element for each LO. The size of this variable and
*lo_multi* shall match.

Unit: Hz

Usage: Set by the user.
*/
ArrayOfGriddedField1& sideband_response_multi(Workspace& ws) noexcept {
  return *static_cast<ArrayOfGriddedField1*>(ws[300]);
}

/*! Specific heat capacity.

It is the heat capacity per unit
mass of a material.
Units: K J^-1 kg^-1

Size: [(cloudbox_limits[1] - cloudbox_limits[0]) +1,
       (cloudbox_limits[3] - cloudbox_limits[2]) +1,
       (cloudbox_limits[5] - cloudbox_limits[4]) +1,
*/
Tensor3& specific_heat_capacity(Workspace& ws) noexcept {
  return *static_cast<Tensor3*>(ws[303]);
}

/*! Spectral irradiance field.

Spectral irradiance is the radiative power per unit area
and unit frequency. The last dimension denotes the hemispheres.
The first component denotes the downward direction and the second
component denotes the upward direction

Units: W m^-2 Hz^-1

 Size: [Nf,
       (cloudbox_limits[1] - cloudbox_limits[0]) +1,
       (cloudbox_limits[3] - cloudbox_limits[2]) +1,
       (cloudbox_limits[5] - cloudbox_limits[4]) +1,
        2]
*/
Tensor5& spectral_irradiance_field(Workspace& ws) noexcept {
  return *static_cast<Tensor5*>(ws[301]);
}

/*! Spectral radiance field.

This variable holds a calculation of the radiance field through
the atmosphere, for the directions matching *za_grid* and *aa_grid*.

Don't confuse this variable with *cloudbox_field*. That varinale also
holds a field of spectral radiances, but is restricted to the cloud box.

Units: W / (m^2 Hz sr)

 Size: [f_grid,
       p_grid,
       lat_grid,
       lon_grid,
       za_grid,
       aa_grid,
       stokes_dim ]

Note: For 1D, the size of the latitude, longitude and azimuth
dimension (N_aa) are all 1.
*/
Tensor7& spectral_radiance_field(Workspace& ws) noexcept {
  return *static_cast<Tensor7*>(ws[302]);
}

/*! The specular direction (for reflection by a flat surface).

The specular direction as a standard line-of-sight vector, consisting
of a zenith and azimuth angle (the later only for 3D).

Units: degrees

Size:  [ 1 or 2 ]
*/
Vector& specular_los(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[304]);
}

/*! Agenda calculating single scattering properties from the amplitude matrix.
 */
Agenda& spt_calc_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[305]);
}

/*! The matrix of total scalar absorption coefficients for source.

Contains the gas absorption summed over all species as a function of
*f_grid* and *abs_p*, i.e., for a single atmospheric profile.

This variable is not used explicitly in a standard calculation, where
absorption comes from the lookup table *abs_lookup*. However, it is
useful for testing the methods that actually calculate line-by-line
absorption, which have this variable as output. These methods are
called internally by the method *abs_lookupCalc*, which generates
the lookup table.

Dimensions: [f_grid, abs_p]

Unit: 1/m
*/
Matrix& src_coef(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[308]);
}

/*! Scalar absorption coefficients individually per tag group.

The Array contains one matrix of absorption coefficients for each
tag group, where the matrix format is the same as that of *src_coef*
*/
ArrayOfMatrix& src_coef_per_species(Workspace& ws) noexcept {
  return *static_cast<ArrayOfMatrix*>(ws[309]);
}

/*! Absorption cross sections for the source function for NLTE calculations.

This variable contains absorption cross section xsec individually for
each tag group. The Array contains one matrix for each tag group, the
matrix format is the same as that of abs_coef.

Dimensions: [abs_species](f_grid, abs_p), or
                         (0, 0)
Unit:       m^2 (alpha = xsec * n * VMR),
            where n is total density.
*/
ArrayOfMatrix& src_xsec_per_species(Workspace& ws) noexcept {
  return *static_cast<ArrayOfMatrix*>(ws[307]);
}

/*! The dimensionality of the Stokes vector (1-4).

Usage:      Set by the user.
*/
Index& stokes_dim(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[306]);
}

/*! Rotation of the Stokes H and V directions.

This variable allows to introduce a rotation of the Stokes coordinate
system. Such a rotation could be needed to handle the scanning
procedure of some instruments, such as AMSU-A. The variable is
applied by the *sensor_responseStokesRotation* WSM.

The rotation is given as an angle for each direction. In general, the
number of rotations to be specified follows *sensor_response_dlos_grid*.
In more detail, if no antenna is included or a 1D antenna is used, and
the rotation is applied before the antenna is included in
*sensor_response*, there should be one angle for each row of
*mblock_dlos_grid*. After inclusion of an antenna response, the relevant
number of angles is determined by the rows of *antenna_dlos*.

It is assumed that the rotation is common for all frequency elements.

Units: degrees

Size:  [ number of directions ]

Usage: Set by the user.
*/
Vector& stokes_rotation(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[310]);
}

/*! Complex refractive index of the surface, at a single point.

See *complex_refr_index* for the expected format and how the data
are treated.
*/
GriddedField3& surface_complex_refr_index(Workspace& ws) noexcept {
  return *static_cast<GriddedField3*>(ws[311]);
}

/*! The emission from the surface.

See specific methods generating *surface_emission* and the user
guide for more information.

Dimensions: [ f_grid, stokes_dim ]
*/
Matrix& surface_emission(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[312]);
}

/*! Downwelling radiation directions to consider in surface reflection.

The directions are given as a zenith and azimuth angle (the later
only for 3D), following the definition of line-of-sights.

Units: degrees

Size:  [ any number, 1 or 2 ]
*/
Matrix& surface_los(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[313]);
}

/*! The normal vector for a point at the surface.

The vector is given as a zenith and azimuth (the later only for 3D)
angle, following the definition of line-of-sights. For example,
this vector is always [0] for 1D, as there is no surface topography
for this atmospheric dimensionality.

Units: degrees

Size:  [ 1 or 2 ]
*/
Vector& surface_normal(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[314]);
}

/*! Various surface properties.

A general container for passing data to surface methods. Each surface
property shall be specified on the grid set by *lat_grid* and *lon_grid*.

The properties are identified by the accompanying variable
*surface_props_names*.

Size:  [ number of props., lat_grid, lon_grid ]
*/
Tensor3& surface_props_data(Workspace& ws) noexcept {
  return *static_cast<Tensor3*>(ws[315]);
}

/*! Name on surface properties found in *surface_props_data*.

Each string names a property in *surface_props_data*. The user is free
to include data with any name, but the surface methods making use of
*surface_props_data* expect data to be named in a specific way. See
the documentation of each method for recognised choices.

Size:  [ number of props. ]
*/
ArrayOfString& surface_props_names(Workspace& ws) noexcept {
  return *static_cast<ArrayOfString*>(ws[316]);
}

/*! Surface reflectivity, for a given position and angle.

This variable describes the surface reflectivity at one position
and one incidence angle. It works as *surface_scalar_reflectivity*
but is also defined for vector radiative transfer.

The first dimension of the variable shall either match *f_grid* or
be 1. The later case is interpreted as the reflectivity is the same
for all frequencies.

Usage:   Input to some surface properties methods.

Dimensions: [ f_grid or 1, stokes_dim, stokes_dim]
*/
Tensor3& surface_reflectivity(Workspace& ws) noexcept {
  return *static_cast<Tensor3*>(ws[322]);
}

/*! The reflection coefficients for the directions given by
*surface_los* to the direction of interest.

The rows and columns of this tensor holds the reflection
coefficient matrix for one frequency and one LOS. The reflection
coefficients shall take into accound the angular weighting of the
downwelling radiation.

See specific methods generating *surface_rmatrix* and the user guide
for more information.

Usage:      Input to methods for *surface_rtprop_agenda*.

Units:      -

Dimensions: [ surface_los, f_grid, stokes_dim, stokes_dim ]
*/
Tensor4& surface_rmatrix(Workspace& ws) noexcept {
  return *static_cast<Tensor4*>(ws[317]);
}

/*! Agenda providing radiative properties of the surface.
 */
Agenda& surface_rtprop_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[318]);
}

/*! Description of surface radiative properties, divided into surface types.
 */
ArrayOfAgenda& surface_rtprop_agenda_array(Workspace& ws) noexcept {
  return *static_cast<ArrayOfAgenda*>(ws[319]);
}

/*! Sub-agenda to *surface_rtprop_agenda*.
 */
Agenda& surface_rtprop_sub_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[320]);
}

/*! Surface reflectivity, described by rv and rh (power) reflectivities.

This variable describes the surface reflectivity at one position
and one incidence angle. For this position and angle, one or multiple
combinations of rv and rh are specified, where rv and rh are the
reflectivity for vertical and horizontal polarisation, respectively.

This matrix shall always have two columns, where the first column
holds rv values, and the second column rh. It is up to the user to
make sure that data are put into the correct column, this can not
be checked bu the methods using this WSV.

The number of rows shall either match *f_grid* or be 1. The later case
is interpreted as the reflectivities are the same for all frequencies.

Usage:   Input to some surface properties methods.

Dimensions: [ f_grid or 1, 2]
*/
Matrix& surface_rv_rh(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[323]);
}

/*! Surface reflectivity, assuming it can be described as a scalar value.

This variable describes the surface reflectivity at one position
and one incidence angle. For this position and angle, one or multiple
scalar reflectivities are specified.

The length of the vector shall either match *f_grid* or be 1. The
later case is interpreted as the reflectivity is the same for all
frequencies (ie. matches a constant vector).

Usage:   Input to some surface properties methods.

Dimensions: [ f_grid or 1]
*/
Vector& surface_scalar_reflectivity(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[324]);
}

/*! Surface skin temperature.

This temperature shall be selected considering the radiative
properties of the surface, and can differ from the "bulk"
temperature.

Usage:   Input to methods for *surface_rtprop_agenda*.
*/
Numeric& surface_skin_t(Workspace& ws) noexcept {
  return *static_cast<Numeric*>(ws[321]);
}

/*! Local surface type value.

See *surface_type_mask* for details.
*/
Index& surface_type(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[325]);
}

/*! Auxiliary variable to *surface_type*.

See *surface_type_mask* for details.
*/
Numeric& surface_type_aux(Workspace& ws) noexcept {
  return *static_cast<Numeric*>(ws[326]);
}

/*! Classification of the surface using a type coding.

This variable gives a description of the surface using a type class
coding. A common term for such a variable is "surface mask".

The mask is a latitude and longtide field. The mask values are
floating numbers, where the integer part is the type and the remainder
can be used to provide auxilary information. In terms of the local
variables, the mask values equal *surface_type* + *surface_type_aux*.

There is no fixed type coding, it is up to the user to set up
a consistent system. The critical point is in the agendas
matching each surface type, that are denoted as iy_surface_sub_agendaX
where X is the *surface_type* index.

The surface type can be any integer (>=0) for which a corresponding
agenda exists.

Dimensions:
   GriddedField2:
      Vector Latitude [N_lat]
      Vector Longitude [N_lon]
      Matrix data [N_lat][N_lon]
*/
GriddedField2& surface_type_mask(Workspace& ws) noexcept {
  return *static_cast<GriddedField2*>(ws[327]);
}

/*! The field of atmospheric temperatures.

This variable gives the atmospheric temperature at each crossing of
the pressure, latitude and longitude grids.

The temperature for a point between the grid crossings is obtained
by (multi-)linear interpolation of the *t_field*.

See further the ARTS user guide (AUG). Use the index to find where
this variable is discussed. The variable is listed as a subentry to
"workspace variables".

Usage:      Output of *AtmFieldsCalc*.

Unit:       K

Dimensions: [ p_grid, lat_grid, lon_grid ]
*/
Tensor3& t_field(Workspace& ws) noexcept {
  return *static_cast<Tensor3*>(ws[338]);
}

/*! Raw data for atmospheric temperatures.

This variable gives the atmospheric temperature as stored in the
database for the atmospheric scenarios.

See further the ARTS user guide (AUG). Use the index to find where
this variable is discussed. The variable is listed as a subentry to
"workspace variables".

Usage: Set by the user by choosing a climatology.

Unit:  K

Size   GriddedField3
        [N_p]
       [N_lat]
       [N_lon]
       [N_p, N_lat, N_lon]
*/
GriddedField3& t_field_raw(Workspace& ws) noexcept {
  return *static_cast<GriddedField3*>(ws[340]);
}

/*! The surface temperature.

This variable holds the temperature of the surface at each latitude
and longitude grid crossing. The normal case should be that this
temperature field is interpolated to obtain *surface_skin_t*.
Accordingly, for 1D cases it could be a better idea to specify
*surface_skin_t* directly.

These temperature shall be selected considering the radiative
properties of the surface, and can differ from the "bulk"
temperatures.

Usage:      Set by user.

Unit:       K

Dimensions: [ lat_grid, lon_grid ]
*/
Matrix& t_surface(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[342]);
}

/*! TELSEM 2 emissivity atlases.

Array should be filled with 12
atlases, one for each month. Index 0 is January, index 11 December.
*/
ArrayOfTelsemAtlas& telsem_atlases(Workspace& ws) noexcept {
  return *static_cast<ArrayOfTelsemAtlas*>(ws[328]);
}

/*! TESSEM2 neural network parameters for horizontal polarization.
 */
TessemNN& tessem_neth(Workspace& ws) noexcept {
  return *static_cast<TessemNN*>(ws[329]);
}

/*! TESSEM2 neural network parameters for vertical polarization.
 */
TessemNN& tessem_netv(Workspace& ws) noexcept {
  return *static_cast<TessemNN*>(ws[330]);
}

/*! A dummy agenda for testing purposes.

Only used for testing by developers.
*/
Agenda& test_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[331]);
}

/*! Array of agenda for TestArrayOfAgenda case.

Only used for testing by developers.
*/
ArrayOfAgenda& test_agenda_array(Workspace& ws) noexcept {
  return *static_cast<ArrayOfAgenda*>(ws[332]);
}

/*! A time point.
 */
Time& time(Workspace& ws) noexcept { return *static_cast<Time*>(ws[333]); }

/*! A grid of times.  Should be increasing
 */
ArrayOfTime& time_grid(Workspace& ws) noexcept {
  return *static_cast<ArrayOfTime*>(ws[335]);
}

/*! A set of times.  Can be in random order
 */
ArrayOfTime& time_stamps(Workspace& ws) noexcept {
  return *static_cast<ArrayOfTime*>(ws[336]);
}

/*! Stores the starting time for time measurements.
 */
Timer& timer(Workspace& ws) noexcept { return *static_cast<Timer*>(ws[334]); }

/*! Transmitter positions.

Used for radio link calculations and gives then the position of the
transmitting device. The corresponding positions of the receiver are
given by *sensor_pos*. The number of rows in *transmitter_pos* and
*sensor_pos* must be equal.

This WSV is also defined as *sensor_pos* regarding the content of the
columns, accepted range for latitudes etc. With one exception, this
WSV is demanded to have two columns also for 1D. The additional
second value is the angular distance between the transmitter and the
reciver. This angle is defined as "latitude" for 2D, with the
sensor fixed at the angle of 0 degree.

Each row this matrix defines *rte_pos2* for the measurement block,
exactly as *sensor_pos* is translated to *rte_pos*.

If no transmitter is involved in the calculations, the variable can
be set to be empty.

Usage: Set by the user.

Unit:  [ m, degrees, degrees ]
*/
Matrix& transmitter_pos(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[337]);
}

/*! ARTS verbosity.

The verbosity variable is implicitly passed to all workspace methods.
It can be used to dynamically control the reporting level during
runtime.

Usage:    Set by user.

See also: *verbosityInit*
          *verbositySet*
          *verbositySetAgenda*
          *verbositySetScreen*
          *verbositySetFile*
*/
Verbosity& verbosity(Workspace& ws) noexcept {
  return *static_cast<Verbosity*>(ws[343]);
}

/*! VMR field.

This variable gives the volume mixing ratio of the chosen gaseous
species as a function of p_grid, lat_grid, lon_grid.

See further the ARTS user guide (AUG). Use the index to find where
this variable is discussed. The variable is listed as a subentry to
"workspace variables".

Units: [ Differ between the elements, can be VMR, kg/m3 or #/m3. ]

Dimensions: [species, p_grid, lat_grid, lon_grid]
*/
Tensor4& vmr_field(Workspace& ws) noexcept {
  return *static_cast<Tensor4*>(ws[344]);
}

/*! VMR data for the chosen gaseous species.

This variable contains the volume mixing ratios (VMR) for all
chosen gaseous species. It includes the grids corresponding to the
grids in the database.
*vmr_field_raw* is an Array of Array of Tensor3. It contains one
gridded field for each species which contains the data and
also the grids.
For the calculation the data is
interpolated on *p_grid*, *lat_grid* and *lon_grid*

Usage: Output of *AtmRawRead*
       Input to *AtmFieldsCalc*.

Unit:  absolute number

Size:  Array[number of absorption species]
       GriddedField3
        [N_p]
       [N_lat]
       [N_lon]
       [N_p, N_lat, N_lon]
*/
ArrayOfGriddedField3& vmr_field_raw(Workspace& ws) noexcept {
  return *static_cast<ArrayOfGriddedField3*>(ws[345]);
}

/*! Agenda to calculate the saturation pressure of water.
 */
Agenda& water_p_eq_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[346]);
}

/*! The field of water saturation pressure.

This variable holds the saturation pressure of water at each crossing of
the pressure, latitude and longitude grids.

Unit:       Pa

Dimensions: [ p_grid, lat_grid, lon_grid ]
*/
Tensor3& water_p_eq_field(Workspace& ws) noexcept {
  return *static_cast<Tensor3*>(ws[347]);
}

/*! Indicates if the wigner tables are initialized.
If they are not, computations will be aborted.

Will hold the value of provided maximum factorial value

The developer should always test this variable in functions
that might require computing wigner symbols because the error
handling is otherwise offloaded to third party software...
*/
Index& wigner_initialized(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[348]);
}

/*! Zonal component of the wind field.

The East-West wind component. Air moving towards higher
longitudes is a positive wind. This wind causes no Doppler shift
for 1D and 2D simulations.

Can be set to be empty, which is interpreted as zero wind speed
everywhere.

Unit:       m/s

Dimensions: [ p_grid, lat_grid, lon_grid ]  or [ 0 0 0 ].
*/
Tensor3& wind_u_field(Workspace& ws) noexcept {
  return *static_cast<Tensor3*>(ws[349]);
}

/*! Raw zonal component of the wind field.

The East-West wind component. Air moving towards higher
longitudes is a positive wind. This wind causes no Doppler shift
for 1D and 2D simulations.

Can be set to be empty, which is interpreted as zero wind speed
everywhere.

Unit:       m/s

Dimensions: [ p_grid, lat_grid, lon_grid ].
*/
GriddedField3& wind_u_field_raw(Workspace& ws) noexcept {
  return *static_cast<GriddedField3*>(ws[350]);
}

/*! Meridional component of the magnetic field.

The North-South wind component. Air moving towards higher
latitudes is a positive wind.

Can be set to be empty, which is interpreted as zero wind speed
everywhere.

Unit:       m/s

Dimensions: [ p_grid, lat_grid, lon_grid ] or [ 0 0 0 ]
*/
Tensor3& wind_v_field(Workspace& ws) noexcept {
  return *static_cast<Tensor3*>(ws[351]);
}

/*! Raw meridional component of the magnetic field.

The North-South wind component. Air moving towards higher
latitudes is a positive wind.

Can be set to be empty, which is interpreted as zero wind speed
everywhere.

Unit:       m/s

Dimensions: [ p_grid, lat_grid, lon_grid ]
*/
GriddedField3& wind_v_field_raw(Workspace& ws) noexcept {
  return *static_cast<GriddedField3*>(ws[352]);
}

/*! Vertical wind component field.

Upward moving air corresponds to a positive wind speed.

Can be set to be empty, which is interpreted as zero wind speed
everywhere.

Unit:       m/s

Dimensions: [ p_grid, lat_grid, lon_grid ] or [ 0 0 0 ]
*/
Tensor3& wind_w_field(Workspace& ws) noexcept {
  return *static_cast<Tensor3*>(ws[353]);
}

/*! Raw vertical wind component field.

Upward moving air corresponds to a positive wind speed.

Can be set to be empty, which is interpreted as zero wind speed
everywhere.

Unit:       m/s

Dimensions: [ p_grid, lat_grid, lon_grid ]
*/
GriddedField3& wind_w_field_raw(Workspace& ws) noexcept {
  return *static_cast<GriddedField3*>(ws[354]);
}

/*! Channel selection for WMRF fast calculation.

This variable can be used to select one or several instrument channels
from the list of all possible channels. Zero-based indexing is used, so
Channel 0 is the first instrument channel!
*/
ArrayOfIndex& wmrf_channels(Workspace& ws) noexcept {
  return *static_cast<ArrayOfIndex*>(ws[355]);
}

/*! The weights for a WMRF fast calculation.

Weights are stored in a sparse matrix. This can be used as a
sensor_response matrix.

The dimension of the matrix is (nchan, nfreq), where nchan
is the number of instrument channels and nfreq is the number
of monochromatic frequencies.
*/
Sparse& wmrf_weights(Workspace& ws) noexcept {
  return *static_cast<Sparse*>(ws[356]);
}

/*! The state vector.

This WSV matches directly the x-vector in the formalism by C.D. Rodgers.

Inside *x*, the elements matching one retrieval quantity, such as
atmospheric temperatures, are kept together. That is, each retrieval
quantity covers a continuous range inside *x*. The start and index of
these ranges can be deduced by *jacobian_quantities* (see function(s)
inside jacobian.cc for details).

The order of elements inside each retrieval quantity should be clarified
by corresponding "adding" method, i.e. *jacobianAddTemperature* for
atmospheric temperatures. The general rule is that data are sorted from
left to right with respect to the order in the corresponding WSV. For
example, inside *x* atmospheric data are stored with pressure as inner-
most loop, followed by latitude and longitude as outermost loop.

Usage: Used by inversion methods.

Unit:  Varies, follows unit of selected retrieval quantities.
*/
Vector& x(Workspace& ws) noexcept { return *static_cast<Vector*>(ws[358]); }

/*! The a priori state vector.

This WSV matches directly the x_a-vector in the formalism by C.D. Rodgers.

Usage: Used by inversion methods.

Unit:  Varies, follows unit of selected retrieval quantities.
*/
Vector& xa(Workspace& ws) noexcept { return *static_cast<Vector*>(ws[359]); }

/*! Flag to determine whether XML output shall be binary or ascii.

This flag has to be set using the workspace method
*output_file_formatSetAscii* or *output_file_formatSetBinary*.
One of these methods MUST be called before writing the first
output file.

Usage: Set by user.
*/
Index& xml_output_type(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[357]);
}

/*! The measurement vector.

This vector holds radiances averaged in frequency and spatially,
and can contain many spectra appended. That is, this WSV matches
directly the y-vector in the formalism by C.D. Rodgers.

The polarisation, frequency, position and line-of-sight associated
with each element in *y* are given by *y_pol*, *y_f*, *y_pos* and
*y_los*. For monochromatic pencil beam radiances, data are sorted
in the following way, from the innermost to the outermost loop
    Stokes
    Frequency
    LOS inside the measurement block
    Measurement block
With sensor response included, the order can be differ. As output
of *yActive*, the order will also be different.

Usage: Output from radiative transfer calculations considering
       sensor response.

Unit:  Undefined. Possibilities include: K, W/(m^2 Hz sr) and
        optical thickness.
*/
Vector& y(Workspace& ws) noexcept { return *static_cast<Vector*>(ws[360]); }

/*! Data auxilary to *y*.

Different data beside the direct result of the radiative transfer
calculations can be obtained by this variable. These auxilary data
are selected by *iy_aux_vars*.

In contrast to *iy_aux*, this variable can only hold quantities such
as optical depth, and other quantites that could be the result
of a complete radiative transfer calculation. The data are weighted
with sensor properties in the same way as for *y*.

See also *iy_aux_vars*.

Usage:      Output of *yCalc*.

Dimensions: [quantity][ element of y ]
*/
ArrayOfVector& y_aux(Workspace& ws) noexcept {
  return *static_cast<ArrayOfVector*>(ws[361]);
}

/*! The baseline of *y*.

In retrieval "lingo", the baseline is an addiative disturbance of
the measured spectrum. That is, it can be seen as a shift (from zero)
of measurement. Reflections inside microwave receivers is one source to
a baseline off-set.

So far there is no module in ARTS that actually tries to physically model
any baseline effect. *y_baseline* is just used as a pure fitting parameter
in retrievals. One example on method to include a baseline fit is
*jacobianAddPolyfit*.

If the baseline is totally constant, it is allowed to set *y_baseline*
to have length one, with this element set to the baseline value.

Usage: Output of retrievals.

Unit:  Same as applied for *y*.
*/
Vector& y_baseline(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[362]);
}

/*! The frequencies associated with *y*.

A value is returned for each element of *y*. Depending on the sensor
set-up and number of measurement blocks, this can be a copy of
*sensor_response_f*, sveral copies of this vector appended, or some
other frequenices.

Don't confuse this variable with *yf*.

Usage: Output from radiative transfer calculations considering
       sensor response.

Unit:  [ Hz ]
*/
Vector& y_f(Workspace& ws) noexcept { return *static_cast<Vector*>(ws[363]); }

/*! The geo-positioning associated with *y*.

The columns of this matrix matches the elements of *geo_pos*.

If no geo-position is obtained (i.e. *geo_pos_agenda* sets *geo_pos*
to be empty), all elements of *y_geo* is set to NaN.

Unit:  [ m, deg, deg, deg, deg ]
*/
Matrix& y_geo(Workspace& ws) noexcept { return *static_cast<Matrix*>(ws[364]); }

/*! The line-of-sights associated with *y*.

Definition of angles matches *sensor_los* (such as first column holds
zenith angles), but gives actual observed LOS. That is, the values of
both *sensor_los* and *antenna_dlos* are considered. Data are provided
for each element of *y*, following y_f, and the number of rows equals
the length of *y*.

Usage: Output from radiative transfer calculations considering
       sensor response.

Unit:  [ degrees, degrees ]
*/
Matrix& y_los(Workspace& ws) noexcept { return *static_cast<Matrix*>(ws[365]); }

/*! The polarisation states associated with *y*.

Data are provided for each element of *y*, following y_f, and the
length of this variable and *y* is equal.

See *instrument_pol* for coding of polarisation components.

Usage: Output from radiative transfer calculations considering
       sensor response.

Unit:  [ - ]
*/
ArrayOfIndex& y_pol(Workspace& ws) noexcept {
  return *static_cast<ArrayOfIndex*>(ws[366]);
}

/*! The sensor positions associated with *y*.

Definition of positions matches *sensor_pos* (such as first column
holds the altitude). Data are provided for each element of *y*,
following y_f, and the number of rows equals the length of *y*.

Usage: Output from radiative transfer calculations considering
       sensor response.

Unit:  [ m, deg, deg ]
*/
Matrix& y_pos(Workspace& ws) noexcept { return *static_cast<Matrix*>(ws[367]); }

/*! The measurement vector for a single measurement block.

Exactly as *y*, but holds data only for a single measurement block.

Usage: Used internally.
*/
Vector& yb(Workspace& ws) noexcept { return *static_cast<Vector*>(ws[368]); }

/*! Batch of spectra.

Each element of *ybatch* corresponds to a spectrum vector *y*.
See further *ybatchCalc*.

Usage: Most commonly produced by *ybatchCalc*.

Unit:  Undefined. Possibilities include: K, W/(m^2 Hz sr) and
        optical thickness.

Dimensions: Number of array elements equals number of batch cases,
            Vectors have length(y)
*/
ArrayOfVector& ybatch(Workspace& ws) noexcept {
  return *static_cast<ArrayOfVector*>(ws[369]);
}

/*! Data auxilary to *ybatch*.

Each element of *ybatch_aux* corresponds to a auxiliary data *y_aux*.
See further *y_aux* and *ybatchCalc*.

Usage: Most commonly produced by *ybatchCalc*.

Dimensions: Number of array elements equals number of batch cases,
*/
ArrayOfArrayOfVector& ybatch_aux(Workspace& ws) noexcept {
  return *static_cast<ArrayOfArrayOfVector*>(ws[370]);
}

/*! Agenda defining the calculations to perform for each batch case.
 */
Agenda& ybatch_calc_agenda(Workspace& ws) noexcept {
  return *static_cast<Agenda*>(ws[371]);
}

/*! Correction terms for *ybatch*.

Dimensions: Number of array elements equals number of batch cases,
            Vectors have length depending on correction method
*/
ArrayOfVector& ybatch_corr(Workspace& ws) noexcept {
  return *static_cast<ArrayOfVector*>(ws[373]);
}

/*! Index of batch case.

See further *ybatchCalc*.

Usage: Set by *ybatchCalc*, for communication with
       *ybatch_calc_agenda*.
*/
Index& ybatch_index(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[372]);
}

/*! All the Jacobians associated with ybatch.

The batch index here is the array dimension.

Usage: Most commonly produced by *ybatch*.

Unit:  Depends on unit of y and on Jacobian type.

Dimensions: [number of batch cases]
             (length(y),
             number of retrieval quantities and grids)
*/
ArrayOfMatrix& ybatch_jacobians(Workspace& ws) noexcept {
  return *static_cast<ArrayOfMatrix*>(ws[374]);
}

/*! Number of batch cases for *ybatchCalc*.

See further *ybatchCalc*.

Usage: Input to *ybatchCalc*.
*/
Index& ybatch_n(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[375]);
}

/*! Start index for *ybatchCalc*.

This is set to a default of zero in *general.arts*.

See further *ybatchCalc*.

Usage: Input to *ybatchCalc*.
*/
Index& ybatch_start(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[376]);
}

/*! A fitted measurement vector.

This WSV is the measurement vector matching the retrieved state, i.e.
the spectrum of the fit.

Don't confuse this variable with *y_f*.

Usage: Output from inversion methods.
*/
Vector& yf(Workspace& ws) noexcept { return *static_cast<Vector*>(ws[377]); }

/*! The field of geometrical altitudes.

This variable gives the geometrical altitude, above the ellipsoid, of
each crossing of the pressure, latitude and longitude grids. For 1D
cases the altitudes give the geometrical position of the pressure
levels.

For each geographical position (lat,lon), the values must be sorted
in increasing order, with no repetitions. Otherwise the altitudes
can be set to arbitrary values. Hydrostatic equilibrium is not
applied automatically. If hydrostatic equilibrium applies, *z_field*
must be set by a method ensuring that this criterium is fulfilled.

The radius (from the coordinate centre) for a point between the grid
crossings is obtained by a (multi-)linear interpolation of the sum
of the ellipsoid radius and *z_field*.

See further the ARTS user guide (AUG). Use the index to find where
this variable is discussed. The variable is listed as a subentry to
"workspace variables".

Usage:      Output of *AtmFieldsCalc*

Unit:       m

Dimensions: [ p_grid, lat_grid, lon_grid ]
*/
Tensor3& z_field(Workspace& ws) noexcept {
  return *static_cast<Tensor3*>(ws[381]);
}

/*! Raw data for geometrical altitudes.

This variable gives the geometrical altitudes as stored in the
database for atmospheric scenarios.

See further the ARTS user guide (AUG). Use the index to find where
this variable is discussed. The variable is listed as a subentry to
"workspace variables".

Usage: Set by the user by choosing a climatology.

Unit:  K

Size   GriddedField3
        [N_p]
       [N_lat]
       [N_lon]
       [N_p, N_lat, N_lon]
*/
GriddedField3& z_field_raw(Workspace& ws) noexcept {
  return *static_cast<GriddedField3*>(ws[382]);
}

/*! Minimum accuracy for calculation of hydrostatic equilibrium.

Usage: Set by the user.

Unit:  m
*/
Numeric& z_hse_accuracy(Workspace& ws) noexcept {
  return *static_cast<Numeric*>(ws[383]);
}

/*! The surface altitude.

This variable defines the shape of the surface, by giving the
geometrical altitude above the geiod for each crossing of the
latitude and longitude grids. Any shape of the surface is accepted.
No gap between the surface and the lowermost pressure level is
allowed.

The radius (from the coordinate centre) for a point between the grid
crossings is obtained by a linear (1D) or bi-linear (2D)
interpolation of the sum of the ellipsoid radius and *z_surface*.
That is, the radius for the surface is assumed to vary linear along
the latitudes and longitudes in *lat_grid* and *lon_grid*.

See further the ARTS user guide (AUG). Use the index to find where
this variable is discussed. The variable is listed as a subentry to
"workspace variables".

Usage:      Set by user.

Unit:       m

Dimensions: [ lat_grid, lon_grid ]
*/
Matrix& z_surface(Workspace& ws) noexcept {
  return *static_cast<Matrix*>(ws[384]);
}

/*! Zenith angle grid.

The zenith angle grid, on which the intensity field is stored.
This grid is used for RT calculations inside the cloudbox, therefore
the grid has to be defined
if the cloudbox is activated by the flag *cloudbox_on*.
The grid must be sorted in increasing order, with no repetitions.

Usage:      Set by the user.

Unit:       degrees
*/
Vector& za_grid(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[378]);
}

/*! TBD.

Unit:  unitless
*/
Vector& za_grid_weights(Workspace& ws) noexcept {
  return *static_cast<Vector*>(ws[379]);
}

/*! Zenith angle index for scattering calculations.

This variable is used internally in WSMs for computing scattering
properties.

Usage:    Input to the agendas *spt_calc_agenda*,
                                *pha_mat_spt_agenda*.
*/
Index& za_index(Workspace& ws) noexcept {
  return *static_cast<Index*>(ws[380]);
}

}  // namespace ARTS::Var

namespace ARTS::Method {
/*! Initialises the WSVs *abs_p*, *abs_t* and *abs_vmrs* from
*p_grid, *t_field* and *vmr_field*.

This only works for a 1D atmosphere!

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void AbsInputFromAtmFields(Workspace& ws) {
  AbsInputFromAtmFields(Var::abs_p(ws), Var::abs_t(ws), Var::abs_vmrs(ws),
                        Var::atmosphere_dim(ws), Var::p_grid(ws),
                        Var::t_field(ws), Var::vmr_field(ws),
                        Var::verbosity(ws));
}

/*! Initialize absorption input WSVs from local atmospheric conditions.

The purpose of this method is to allow an explicit line-by-line
calculation, e.g., by *abs_coefCalcFromXsec*, to be put inside the
*propmat_clearsky_agenda*. What the method does is to prepare absorption
input parameters (pressure, temperature, VMRs), from the input
parameters to *propmat_clearsky_agenda*.

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void AbsInputFromRteScalars(Workspace& ws) {
  AbsInputFromRteScalars(Var::abs_p(ws), Var::abs_t(ws), Var::abs_vmrs(ws),
                         Var::rtp_pressure(ws), Var::rtp_temperature(ws),
                         Var::rtp_vmr(ws), Var::verbosity(ws));
}

/*! Creates a variable of group AbsorptionLines.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void AbsorptionLinesCreate(Workspace& ws, AbsorptionLines& out) {
  AbsorptionLinesCreate(out, Var::verbosity(ws));
}

/*! Adds zenith and azimuth angles.

Adds up line-of-sights (LOS). In short, *dlos* is added to *ref_los*,
assuming that a unit changes in zenith and azimuth are equal where
dlos=(0,0).

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] new_los - End line-of-sights.
@param[in] ref_los - Reference line-of-sight (a single LOS).
@param[in] dlos - Change in line-of-sight (can be multiple LOS).

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void AddZaAa(Workspace& ws, Matrix& new_los, const Vector& ref_los,
             const Matrix& dlos) {
  AddZaAa(new_los, ref_los, dlos, Var::verbosity(ws));
}

/*! Creates a variable of group Agenda.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void AgendaCreate(Workspace& ws, Agenda& out) {
  AgendaCreate(out, Var::verbosity(ws));
}

/*! Execute an agenda.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] a - Agenda to be executed.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void AgendaExecute(Workspace& ws, const Agenda& a) {
  AgendaExecute(ws, a, Var::verbosity(ws));
}

/*! Execute an agenda exclusively.

Only one call to *AgendaExecuteExclusive* is executed at a time.
Other calls to this function are blocked until the current one
finishes. WARNING: Can cause deadlocks! Use with care.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] a - Agenda to be executed.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void AgendaExecuteExclusive(Workspace& ws, const Agenda& a) {
  AgendaExecuteExclusive(ws, a, Var::verbosity(ws));
}

/*! Sets the angular grids for the calculation of radiation fluxes
(irradiance) per hemispheres and heating rates

This method sets the angular grids for the radiation fluxes type
calculations and calculates the integration weights *za_grid_weights*
for the zenith angle integration. For down- und up-looking
geometries it suffices to define *N_za_grid* and
*N_aa_grid*. From *N_aa_grid* an equally spaced grid is
created and stored in the WSV*aa_grid*.
Depending on the desired *za_grid_type* *za_grid* will be
equally spaced ('linear') or unequally ('linear_mu','double_gauss')
Important, *N_za_grid* must be an even number because for the
integration over each hemisphere *N_za_grid* / 2 zenith angles are needed.

Possible zenith angle grid types are:
double_gauss:     The zenith grid and the integration weights are set according
                  to a gauss-legendre integration for each hemispheres.
linear:           Equally space grid between 0 deg and 180 deg including the
poles linear_mu:        Similar to 'linear' but equally spaced for cos(180 deg)
to cos(0 deg), which results a unequally spaced angular grid

@author Manfred Brath

@param[in,out] Workspace ws - An ARTS workspace
@param[in] N_za_grid - Number of zenith angles (default: 2)
@param[in] N_aa_grid - Number of azimuth angles (default: 1)
@param[in] za_grid_type - Zenith angle grid type (default: "linear_mu")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void AngularGridsSetFluxCalc(Workspace& ws, const Index& N_za_grid = 2,
                             const Index& N_aa_grid = 1,
                             const String& za_grid_type = "linear_mu") {
  AngularGridsSetFluxCalc(Var::za_grid(ws), Var::aa_grid(ws),
                          Var::za_grid_weights(ws), N_za_grid, N_aa_grid,
                          za_grid_type, Var::verbosity(ws));
}

/*! Sets up a 1D gaussian antenna response and a matching
*mblock_dlos_grid*.

As *antenna_responseGaussian*, but also creates *mblock_dlos_grid*.
For returned antenna response, see *antenna_responseGaussian*.

The size of *mblock_dlos_grid* is determined by *n_za_grid*.
The end points of the grid are set to be the same as for the
antenna response. The spacing of the grid follows the magnitude of
the response; the spacing is smaller where the response is high.
More precisely, the grid points are determined by dividing
the cumulative sum of the response in equal steps. This makes sense
if the representation error of the radiance (as a function of
zenith angle) increases linearly with the grid spacing.

The WSV *antenna_dlos* is set to [0].

The antenna repsonse is not normalised.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] n_za_grid - Number of points to include in *mblock_dlos_grid*.
@param[in] fwhm - Full width at half-maximum of antenna beam [deg].
@param[in] xwidth_si - Half-width of response, in terms of std. dev. (default:
3)
@param[in] dx_si - Grid spacing, in terms of std. dev. (default: 0.1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void AntennaConstantGaussian1D(Workspace& ws, const Index& n_za_grid,
                               const Numeric& fwhm,
                               const Numeric& xwidth_si = 3,
                               const Numeric& dx_si = 0.1) {
  AntennaConstantGaussian1D(Var::antenna_dim(ws), Var::mblock_dlos_grid(ws),
                            Var::antenna_response(ws), Var::antenna_dlos(ws),
                            n_za_grid, fwhm, xwidth_si, dx_si,
                            Var::verbosity(ws));
}

/*! Maps a multi-beam case to a matching pencil beam case.

Cases with overlapping beams are most efficiently handled by
letting *antenna_dlos* have several rows. That is, there are
multiple beams for each measurement block. The drawback is that
many variables must be adjusted if the corresponding pencil beam
spectra shall be calculated. This method makes this adjustment.
That is, if you have a control file for a multiple beam case and
for some reason want to avoid the antenna weighting, you add this
method before *sensor_responseInit*, and remove the call of
*sensor_responseAntenna* and you will get the matching pencil beam
spectra.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void AntennaMultiBeamsToPencilBeams(Workspace& ws) {
  AntennaMultiBeamsToPencilBeams(Var::sensor_pos(ws), Var::sensor_los(ws),
                                 Var::antenna_dlos(ws), Var::antenna_dim(ws),
                                 Var::mblock_dlos_grid(ws),
                                 Var::atmosphere_dim(ws), Var::verbosity(ws));
}

/*! Sets some antenna related variables

Use this method to set *antenna_dim* and *mblock_dlos_grid* to
suitable values (1 and [0], respectively) for cases when a
sensor is included, but the antenna pattern is neglected.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void AntennaOff(Workspace& ws) {
  AntennaOff(Var::antenna_dim(ws), Var::mblock_dlos_grid(ws),
             Var::verbosity(ws));
}

/*! Append one workspace variable to another.

This method can append an array to another array of the same type,
e.g. ArrayOfIndex to ArrayOfIndex. Or a single element to an array
such as a Tensor3 to an ArrayOfTensor3.

Appending two vectors or a numeric to a vector works as for array
variables.

Both another matrix or a vector can be appended to a matrix. In
addition, for matrices, the 'append dimension' can be selected.
The third argument, *dimension*, indicates how to append, where
"leading" means to append row-wise, and "trailing" means
column-wise.

Other types (TensorX) are currently only implemented for
appending to the leading dimension.

This method is not implemented for all types, just for those that
were thought or found to be useful. (See variable list below.).

@author Stefan Buehler, Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - The variable to append to.
@param[in] in - The variable to append.
@param[in] dimension - Where to append. Could be either the "leading" or
"trailing" dimension. (default: "leading")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0, typename Any_1>
void Append(Workspace& ws, std::pair<Any_0, String>& out,
            const std::pair<Any_1, String>& in,
            const std::pair<String, String>& dimension = {"leading",
                                                          "dimension"}) {
  Append(out.first, out.second, in.first, dimension.first, in.second,
         dimension.second, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfAbsorptionLines.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfAbsorptionLinesCreate(Workspace& ws, ArrayOfAbsorptionLines& out) {
  ArrayOfAbsorptionLinesCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfAgenda.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfAgendaCreate(Workspace& ws, ArrayOfAgenda& out) {
  ArrayOfAgendaCreate(out, Var::verbosity(ws));
}

/*! Execute an agenda from an ArrayOfAgenda.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] agendas - Array of agendas.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfAgendaExecute(Workspace& ws, const ArrayOfAgenda& agendas) {
  ArrayOfAgendaExecute(ws, Var::agenda_array_index(ws), agendas,
                       Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfArrayOfAbsorptionLines.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfArrayOfAbsorptionLinesCreate(Workspace& ws,
                                         ArrayOfArrayOfAbsorptionLines& out) {
  ArrayOfArrayOfAbsorptionLinesCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfArrayOfGriddedField1.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfArrayOfGriddedField1Create(Workspace& ws,
                                       ArrayOfArrayOfGriddedField1& out) {
  ArrayOfArrayOfGriddedField1Create(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfArrayOfGriddedField2.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfArrayOfGriddedField2Create(Workspace& ws,
                                       ArrayOfArrayOfGriddedField2& out) {
  ArrayOfArrayOfGriddedField2Create(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfArrayOfGriddedField3.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfArrayOfGriddedField3Create(Workspace& ws,
                                       ArrayOfArrayOfGriddedField3& out) {
  ArrayOfArrayOfGriddedField3Create(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfArrayOfIndex.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfArrayOfIndexCreate(Workspace& ws, ArrayOfArrayOfIndex& out) {
  ArrayOfArrayOfIndexCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfArrayOfMatrix.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfArrayOfMatrixCreate(Workspace& ws, ArrayOfArrayOfMatrix& out) {
  ArrayOfArrayOfMatrixCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfArrayOfPropagationMatrix.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfArrayOfPropagationMatrixCreate(
    Workspace& ws, ArrayOfArrayOfPropagationMatrix& out) {
  ArrayOfArrayOfPropagationMatrixCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfArrayOfRadiationVector.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfArrayOfRadiationVectorCreate(Workspace& ws,
                                         ArrayOfArrayOfRadiationVector& out) {
  ArrayOfArrayOfRadiationVectorCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfArrayOfScatteringMetaData.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfArrayOfScatteringMetaDataCreate(
    Workspace& ws, ArrayOfArrayOfScatteringMetaData& out) {
  ArrayOfArrayOfScatteringMetaDataCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfArrayOfSingleScatteringData.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfArrayOfSingleScatteringDataCreate(
    Workspace& ws, ArrayOfArrayOfSingleScatteringData& out) {
  ArrayOfArrayOfSingleScatteringDataCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfArrayOfSpeciesTag.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfArrayOfSpeciesTagCreate(Workspace& ws,
                                    ArrayOfArrayOfSpeciesTag& out) {
  ArrayOfArrayOfSpeciesTagCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfArrayOfStokesVector.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfArrayOfStokesVectorCreate(Workspace& ws,
                                      ArrayOfArrayOfStokesVector& out) {
  ArrayOfArrayOfStokesVectorCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfArrayOfString.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfArrayOfStringCreate(Workspace& ws, ArrayOfArrayOfString& out) {
  ArrayOfArrayOfStringCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfArrayOfTensor3.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfArrayOfTensor3Create(Workspace& ws, ArrayOfArrayOfTensor3& out) {
  ArrayOfArrayOfTensor3Create(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfArrayOfTensor6.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfArrayOfTensor6Create(Workspace& ws, ArrayOfArrayOfTensor6& out) {
  ArrayOfArrayOfTensor6Create(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfArrayOfTime.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfArrayOfTimeCreate(Workspace& ws, ArrayOfArrayOfTime& out) {
  ArrayOfArrayOfTimeCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfArrayOfTransmissionMatrix.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfArrayOfTransmissionMatrixCreate(
    Workspace& ws, ArrayOfArrayOfTransmissionMatrix& out) {
  ArrayOfArrayOfTransmissionMatrixCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfArrayOfVector.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfArrayOfVectorCreate(Workspace& ws, ArrayOfArrayOfVector& out) {
  ArrayOfArrayOfVectorCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfCIARecord.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfCIARecordCreate(Workspace& ws, ArrayOfCIARecord& out) {
  ArrayOfCIARecordCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfGriddedField1.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfGriddedField1Create(Workspace& ws, ArrayOfGriddedField1& out) {
  ArrayOfGriddedField1Create(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfGriddedField2.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfGriddedField2Create(Workspace& ws, ArrayOfGriddedField2& out) {
  ArrayOfGriddedField2Create(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfGriddedField3.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfGriddedField3Create(Workspace& ws, ArrayOfGriddedField3& out) {
  ArrayOfGriddedField3Create(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfGriddedField4.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfGriddedField4Create(Workspace& ws, ArrayOfGriddedField4& out) {
  ArrayOfGriddedField4Create(out, Var::verbosity(ws));
}

/*! Get the names of all GriddedFields stored in an Array.

See *GriddedFieldGetName*.

@author Lukas Kluft

@param[in,out] Workspace ws - An ARTS workspace
@param[out] names - Names of the GriddedFields in the ArrayOfGriddedField.
@param[in] griddedfields - Array of GriddedFields.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void ArrayOfGriddedFieldGetNames(Workspace& ws, ArrayOfString& names,
                                 const Any_0& griddedfields) {
  ArrayOfGriddedFieldGetNames(names, griddedfields, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfIndex.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfIndexCreate(Workspace& ws, ArrayOfIndex& out) {
  ArrayOfIndexCreate(out, Var::verbosity(ws));
}

/*! Initializes an ArrayOfIndex with linear spacing.

The first element equals always the start value, and the spacing
equals always the step value, but the last value can deviate from
the stop value. *step* can be both positive and negative.

The created array is [start, start+step, start+2*step, ...]

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Output array.
@param[in] start - Start value.
@param[in] stop - Maximum/minimum value of the end value
@param[in] step - Spacing of the array.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfIndexLinSpace(Workspace& ws, ArrayOfIndex& out, const Index& start,
                          const Index& stop, const Index& step) {
  ArrayOfIndexLinSpace(out, start, stop, step, Var::verbosity(ws));
}

/*! Creates an ArrayOfIndex from the given list of numbers.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to initialize.
@param[in] value - Indexes for initializiation.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfIndexSet(Workspace& ws, ArrayOfIndex& out,
                     const ArrayOfIndex& value) {
  ArrayOfIndexSet(out, value, Var::verbosity(ws));
}

/*! Creates an ArrayOfIndex of length *nelem*, with all values
identical.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to initialize.
@param[in] value - Array value..

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfIndexSetConstant(Workspace& ws, ArrayOfIndex& out,
                             const Index& value) {
  ArrayOfIndexSetConstant(out, Var::nelem(ws), value, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfMatrix.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfMatrixCreate(Workspace& ws, ArrayOfMatrix& out) {
  ArrayOfMatrixCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfPpath.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfPpathCreate(Workspace& ws, ArrayOfPpath& out) {
  ArrayOfPpathCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfPropagationMatrix.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfPropagationMatrixCreate(Workspace& ws,
                                    ArrayOfPropagationMatrix& out) {
  ArrayOfPropagationMatrixCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfQuantumIdentifier.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfQuantumIdentifierCreate(Workspace& ws,
                                    ArrayOfQuantumIdentifier& out) {
  ArrayOfQuantumIdentifierCreate(out, Var::verbosity(ws));
}

/*! Sets an ArrayOfQuantumIdentifier to all levels in *abs_lines_per_species*
with defined quantum numbers

Lines without defined quantum numbers are ignored

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Identifiers to all levels in *abs_lines_per_species*
@param[in] global - Only look at global quantum numbers (default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfQuantumIdentifierFromLines(Workspace& ws,
                                       ArrayOfQuantumIdentifier& out,
                                       const Index& global = 1) {
  ArrayOfQuantumIdentifierFromLines(out, Var::abs_lines_per_species(ws), global,
                                    Var::verbosity(ws));
}

/*! Sets an ArrayOfQuantumIdentifier workspace variable to the given value
by converting the input ArrayOfString

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variables to initialize.
@param[in] string_initializers - The array of string representing the values of
the array.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfQuantumIdentifierSet(Workspace& ws, ArrayOfQuantumIdentifier& out,
                                 const ArrayOfString& string_initializers) {
  ArrayOfQuantumIdentifierSet(out, string_initializers, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfRadiationVector.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfRadiationVectorCreate(Workspace& ws, ArrayOfRadiationVector& out) {
  ArrayOfRadiationVectorCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfRetrievalQuantity.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfRetrievalQuantityCreate(Workspace& ws,
                                    ArrayOfRetrievalQuantity& out) {
  ArrayOfRetrievalQuantityCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfScatteringMetaData.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfScatteringMetaDataCreate(Workspace& ws,
                                     ArrayOfScatteringMetaData& out) {
  ArrayOfScatteringMetaDataCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfSingleScatteringData.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfSingleScatteringDataCreate(Workspace& ws,
                                       ArrayOfSingleScatteringData& out) {
  ArrayOfSingleScatteringDataCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfSparse.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfSparseCreate(Workspace& ws, ArrayOfSparse& out) {
  ArrayOfSparseCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfStokesVector.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfStokesVectorCreate(Workspace& ws, ArrayOfStokesVector& out) {
  ArrayOfStokesVectorCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfString.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfStringCreate(Workspace& ws, ArrayOfString& out) {
  ArrayOfStringCreate(out, Var::verbosity(ws));
}

/*! Sets a String array according the given text.
The format is text = ["String1","String2",...]

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to initialize.
@param[in] value - Strings for initialization.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfStringSet(Workspace& ws, ArrayOfString& out,
                      const ArrayOfString& value) {
  ArrayOfStringSet(out, value, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfTelsemAtlas.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfTelsemAtlasCreate(Workspace& ws, ArrayOfTelsemAtlas& out) {
  ArrayOfTelsemAtlasCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfTensor3.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfTensor3Create(Workspace& ws, ArrayOfTensor3& out) {
  ArrayOfTensor3Create(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfTensor4.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfTensor4Create(Workspace& ws, ArrayOfTensor4& out) {
  ArrayOfTensor4Create(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfTensor5.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfTensor5Create(Workspace& ws, ArrayOfTensor5& out) {
  ArrayOfTensor5Create(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfTensor6.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfTensor6Create(Workspace& ws, ArrayOfTensor6& out) {
  ArrayOfTensor6Create(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfTensor7.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfTensor7Create(Workspace& ws, ArrayOfTensor7& out) {
  ArrayOfTensor7Create(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfTime.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfTimeCreate(Workspace& ws, ArrayOfTime& out) {
  ArrayOfTimeCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfTransmissionMatrix.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfTransmissionMatrixCreate(Workspace& ws,
                                     ArrayOfTransmissionMatrix& out) {
  ArrayOfTransmissionMatrixCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfVector.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfVectorCreate(Workspace& ws, ArrayOfVector& out) {
  ArrayOfVectorCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group ArrayOfXsecRecord.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ArrayOfXsecRecordCreate(Workspace& ws, ArrayOfXsecRecord& out) {
  ArrayOfXsecRecordCreate(out, Var::verbosity(ws));
}

/*! Interpolates the input field along the pressure dimension from
*p_grid_old* to to *p_grid_new*.

Extrapolation is allowed within the common 0.5grid-step margin.
in and out fields can be the same variable.

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Regridded atmospheric field.
@param[in] in - Input atmospheric field.
@param[in] p_grid_new - Pressure grid to regrid to
@param[in] p_grid_old - Pressure grid of input field
@param[in] interp_order - Interpolation order. (default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0, typename Any_1>
void AtmFieldPRegrid(Workspace& ws, Any_0& out, const Any_1& in,
                     const Vector& p_grid_new, const Vector& p_grid_old,
                     const Index& interp_order = 1) {
  AtmFieldPRegrid(out, in, p_grid_new, p_grid_old, interp_order,
                  Var::verbosity(ws));
}

/*! Adds a perturbation to an atmospheric field.

The shape and position of the perturbation follow the retrieval grids.
That is, the shape of the perturbation has a traingular shape,
with breake points at the retrieval grid points. The position is
given as an index. This index matches the column in the Jacobian
for the selected grid position.

If the retrieval grids fully match the atmospheric grids, you can
use *AtmFieldPerturbAtmGrids*, that is faster. The description of
that method can help to understand this method.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] perturbed_field - Perturbed/modified field.
@param[in] original_field - Original field, e.g. *t_field*.
@param[in] p_ret_grid - Pressure retrieval grid.
@param[in] lat_ret_grid - Latitude retrieval grid.
@param[in] lon_ret_grid - Longitude retrieval grid.
@param[in] pert_index - Index of position where the perturbation shall be
performed.
@param[in] pert_size - Size of perturbation.
@param[in] pert_mode - Type of perturbation, absolute or relative. (default:
"absolute")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void AtmFieldPerturb(Workspace& ws, Tensor3& perturbed_field,
                     const Tensor3& original_field, const Vector& p_ret_grid,
                     const Vector& lat_ret_grid, const Vector& lon_ret_grid,
                     const Index& pert_index, const Numeric& pert_size,
                     const String& pert_mode = "absolute") {
  AtmFieldPerturb(perturbed_field, Var::atmosphere_dim(ws), Var::p_grid(ws),
                  Var::lat_grid(ws), Var::lon_grid(ws), original_field,
                  p_ret_grid, lat_ret_grid, lon_ret_grid, pert_index, pert_size,
                  pert_mode, Var::verbosity(ws));
}

/*! As *AtmFieldPerturb*, but perturbation follows the atmospheric grids.

The method effectively performs this
  perturbed_field = original_field
  perturbed_field(p_index,lat_index,lon_index) += pert_size
if not *pert_mode* is set to relative when this is done
  perturbed_field = original_field
  perturbed_field(p_index,lat_index,lon_index) *= 1*pert_size
where p_index etc. are derived from *pert_index*.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] perturbed_field - Perturbed/modified field.
@param[in] original_field - Original field, e.g. *t_field*.
@param[in] pert_index - Index of position where the perturbation shall be
performed.
@param[in] pert_size - Size of perturbation.
@param[in] pert_mode - Type of perturbation, ansolute or relative. (default:
"absolute")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void AtmFieldPerturbAtmGrids(Workspace& ws, Tensor3& perturbed_field,
                             const Tensor3& original_field,
                             const Index& pert_index, const Numeric& pert_size,
                             const String& pert_mode = "absolute") {
  AtmFieldPerturbAtmGrids(perturbed_field, Var::atmosphere_dim(ws),
                          Var::p_grid(ws), Var::lat_grid(ws), Var::lon_grid(ws),
                          original_field, pert_index, pert_size, pert_mode,
                          Var::verbosity(ws));
}

/*! Extract pressure grid and atmospheric fields from
*atm_fields_compact*.

An atmospheric scenario includes the following data for each
position (pressure, latitude, longitude) in the atmosphere:
           1. temperature field
           2. the corresponding altitude field
           3. vmr fields for the gaseous species
           4. scattering species fields

This method splits up the data found in *atm_fields_compact* to
p_grid, lat_grid, lon_grid, vmr_field, particle_bulkprop_field,
and particle_bulkprop_names.
See documentation of *atm_fields_compact* for a definition of the
data.

Compact states are characterized by having all atmospheric fields
already given on identical grids. That is, no interpolation needs
to be and is performed. Keyword *p_min* allows to remove atmospheric
levels with pressures lower than the given value (default: no
removal). This reduces computational burden and is useful when
upper atmospheric contributions are negligible.

Possible future extensions: Add a keyword parameter to refine the
pressure grid if it is too coarse. Or a version that interpolates
onto given grids, instead of using and returning the original grids.

@author Jana Mendrok, Manfred Brath

@param[in,out] Workspace ws - An ARTS workspace
@param[in] delim - Delimiter string of *scat_species* elements. (default: "-")
@param[in] p_min - Minimum-pressure level to consider (for TOA). (default: 0)
@param[in] check_gridnames - A flag with value 1 or 0. If set to one, the
gridnames of the *atm_fields_compact* are checked. (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void AtmFieldsAndParticleBulkPropFieldFromCompact(
    Workspace& ws, const String& delim = "-", const Numeric& p_min = 0,
    const Index& check_gridnames = 0) {
  AtmFieldsAndParticleBulkPropFieldFromCompact(
      Var::p_grid(ws), Var::lat_grid(ws), Var::lon_grid(ws), Var::t_field(ws),
      Var::z_field(ws), Var::vmr_field(ws), Var::particle_bulkprop_field(ws),
      Var::particle_bulkprop_names(ws), Var::abs_species(ws),
      Var::atm_fields_compact(ws), Var::atmosphere_dim(ws), delim, p_min,
      check_gridnames, Var::verbosity(ws));
}

/*! Interpolation of raw atmospheric T, z, VMR, and NLTE T/r fields to
calculation grids.

An atmospheric scenario includes the following data for each
position (pressure, latitude, longitude) in the atmosphere:
   1. temperature field
   2. the corresponding altitude field
   3. vmr fields for the gaseous species
This method interpolates the fields of raw data (*t_field_raw*,
*z_field_raw*, *vmr_field_raw*) which can be stored on arbitrary
grids to the calculation grids (*p_grid*, *lat_grid*, *lon_grid*).
If *nlte_field_raw* is empty, it is assumed to be so because LTE is
assumed by the user and *nlte_field* will be empty.

Internally, *AtmFieldsCalc* applies *GriddedFieldPRegrid* and
*GriddedFieldLatLonRegrid*. Generally, 'half-grid-step' extrapolation
is allowed and applied. However, if *vmr_zeropadding*=1 then VMRs at
*p_grid* levels exceeding the raw VMRs' pressure grid are set to 0
(applying the *vmr_zeropadding* option of *GriddedFieldPRegrid*).

Default is to just accept obtained VMRs. If you want to enforce
that all VMR created are >= 0, set *vmr_nonegative* to 1. Negative
values are then set 0. Beside being present in input data, negative
VMR can be generated from the interpolation if *interp_order* is
above 1.

@author Claudia Emde
@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace
@param[in] interp_order - Interpolation order (1=linear interpolation).
(default: 1)
@param[in] vmr_zeropadding - Pad VMRs with zeroes to fit the pressure grid if
necessary. (default: 0)
@param[in] vmr_nonegative - If set to 1, negative VMRs are set to 0. (default:
0)
@param[in] nlte_when_negative - -1: Skip step. 0: Negative is 0. Else: Negative
is t. (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void AtmFieldsCalc(Workspace& ws, const Index& interp_order = 1,
                   const Index& vmr_zeropadding = 0,
                   const Index& vmr_nonegative = 0,
                   const Index& nlte_when_negative = 0) {
  AtmFieldsCalc(Var::t_field(ws), Var::z_field(ws), Var::vmr_field(ws),
                Var::nlte_field(ws), Var::p_grid(ws), Var::lat_grid(ws),
                Var::lon_grid(ws), Var::t_field_raw(ws), Var::z_field_raw(ws),
                Var::vmr_field_raw(ws), Var::nlte_field_raw(ws),
                Var::nlte_level_identifiers(ws),
                Var::nlte_vibrational_energies(ws), Var::atmosphere_dim(ws),
                interp_order, vmr_zeropadding, vmr_nonegative,
                nlte_when_negative, Var::verbosity(ws));
}

/*! Interpolation of 1D raw atmospheric fields to create 2D or 3D
homogeneous atmospheric fields.

The method works as *AtmFieldsCalc*, but accepts only raw 1D
atmospheres. The raw atmosphere is interpolated to *p_grid* and
the obtained values are applied for all latitudes, and also
longitudes for 3D, to create a homogeneous atmosphere.

The method deals only with the atmospheric fields, and to create
a true 2D or 3D version of a 1D case, a demand is also that the
ellipsoid is set to be a sphere.

@author Patrick Eriksson
@author Claudia Emde
@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace
@param[in] interp_order - Interpolation order (1=linear interpolation).
(default: 1)
@param[in] vmr_zeropadding - Pad VMRs with zeroes to fit the pressure grid if
necessary. (default: 0)
@param[in] vmr_nonegative - If set to 1, negative VMRs are set to 0. (default:
0)
@param[in] nlte_when_negative - -1: Skip step. 0: Negative is 0. Else: Negative
is t. (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void AtmFieldsCalcExpand1D(Workspace& ws, const Index& interp_order = 1,
                           const Index& vmr_zeropadding = 0,
                           const Index& vmr_nonegative = 0,
                           const Index& nlte_when_negative = 0) {
  AtmFieldsCalcExpand1D(
      Var::t_field(ws), Var::z_field(ws), Var::vmr_field(ws),
      Var::nlte_field(ws), Var::p_grid(ws), Var::lat_grid(ws),
      Var::lon_grid(ws), Var::t_field_raw(ws), Var::z_field_raw(ws),
      Var::vmr_field_raw(ws), Var::nlte_field_raw(ws),
      Var::nlte_level_identifiers(ws), Var::nlte_vibrational_energies(ws),
      Var::atmosphere_dim(ws), interp_order, vmr_zeropadding, vmr_nonegative,
      nlte_when_negative, Var::verbosity(ws));
}

/*! Maps a 1D case to 2D or 3D homogeneous atmospheric fields.

This method takes a 1D atmospheric case and converts it to the
corresponding case for 2D or 3D. The atmospheric fields (t_field,
z_field and vmr_field) must be 1D and match *p_grid*. The size of
the new data is determined by *atmosphere_dim*, *lat_grid* and
*lon_grid*. That is, these later variables have been changed since
the original fields were created.

The method deals only with the atmospheric fields, and to create
a true 2D or 3D version of a 1D case, a demand is also that the
ellipsoid is set to be a sphere.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] chk_vmr_nan - Flag to determine if a search for NaN shall be
performed or not. (default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void AtmFieldsExpand1D(Workspace& ws, const Index& chk_vmr_nan = 1) {
  AtmFieldsExpand1D(Var::t_field(ws), Var::z_field(ws), Var::vmr_field(ws),
                    Var::p_grid(ws), Var::lat_grid(ws), Var::lon_grid(ws),
                    Var::atmosphere_dim(ws), chk_vmr_nan, Var::verbosity(ws));
}

/*! Converts 2D or 3D homogeneous atmospheric fields to a 1D case.

The method extracts data for given latitude and longitude index
to create a 1D atmosphere. *AtmosphereSet1D* is called to set
output values of *atmosphere_dim*, *lat_grid* and *lon_grid*.
Nothing is done if *atmosphere_dim* alöready is 1.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] ilat - Pick data having this latitude index (0-based). (default: 0)
@param[in] ilon - Pick data having this longitude index (0-based). (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void AtmFieldsExtract1D(Workspace& ws, const Index& ilat = 0,
                        const Index& ilon = 0) {
  AtmFieldsExtract1D(Var::atmosphere_dim(ws), Var::lat_grid(ws),
                     Var::lon_grid(ws), Var::t_field(ws), Var::z_field(ws),
                     Var::vmr_field(ws), ilat, ilon, Var::verbosity(ws));
}

/*! Refines the pressure grid and regrids the clearsky atmospheric
fields accordingly.

This method is, e.g., used for absorption lookup table testing. It
can also be used to refine the *p_grid* and atmospheric fields from
compact state atmospheres.

It adds additional vertical grid points to the atmospheric fields, by
interpolating them in the usual ARTS way (linear in log pressure).

How fine the new grid will be is determined by the keyword parameter
p_step. The definition of p_step, and the default interpolation
behavior, is consistent with *abs_lookupSetup* and
*abs_lookupSetupBatch* (new points are added between the original
ones, so that the spacing is always below p_step.)

Internally, *AtmFieldsRefinePgrid* applies *p_gridRefine* and
*AtmFieldPRegrid* to the clearsky atmospheric fields (T, z, vmr).

Atmospheric field related check WSV are reset to 0 (unchecked),
i.e., the corresponding checkedCalc methods have to be performed
(again) before *yCalc* or similar methods can be executed.

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace
@param[in] p_step - Maximum step in log(p[Pa]) (natural logarithm, as always).
If the pressure grid is coarser than this, additional points are added until
each log step is smaller than this.

@param[in] interp_order - Interpolation order. (default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void AtmFieldsRefinePgrid(Workspace& ws, const Numeric& p_step,
                          const Index& interp_order = 1) {
  AtmFieldsRefinePgrid(
      Var::p_grid(ws), Var::t_field(ws), Var::z_field(ws), Var::vmr_field(ws),
      Var::atmfields_checked(ws), Var::atmgeom_checked(ws),
      Var::cloudbox_checked(ws), Var::lat_grid(ws), Var::lon_grid(ws),
      Var::atmosphere_dim(ws), p_step, interp_order, Var::verbosity(ws));
}

/*! Reads atmospheric data from a scenario.

An atmospheric scenario includes the following data for each
position (pressure, latitude, longitude) in the atmosphere:
   1. temperature field
   2. the corresponding altitude field
   3. vmr fields for the absorption species
The vmr fields read are governed by the species given in
*abs_species*. Beside gaseous species, these can also contain
purely absorbing particulate matter. In the latter case the
profiles are supposed to provide the mass content (unit kg/m3) for
clouds and precipitation rate (unit kg/m2/s) for precipitation
instead of the vmr.

The data is stored in different files. This methods reads all
files and creates the variables *t_field_raw*, *z_field_raw* and
*vmr_field_raw*.  *nlte_field_raw* is set to empty.

Files in a scenarios should be named matching the pattern of:
basename.speciesname.xml
 (for temperature and altitude the
expected 'speciesname' are 't' and'z', respectivly).
The files can be anywhere, but they must all be in the same
directory, selected by 'basename'. The files are chosen by the
species name. If you have more than one tag group for the same
species, the same profile will be used.

@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace
@param[in] basename - Name of scenario, probably including the full path. For
example: "/smiles_local/arts-data/atmosphere/fascod/tropical"

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void AtmRawRead(Workspace& ws, const String& basename) {
  AtmRawRead(Var::t_field_raw(ws), Var::z_field_raw(ws), Var::vmr_field_raw(ws),
             Var::nlte_field_raw(ws), Var::nlte_level_identifiers(ws),
             Var::nlte_vibrational_energies(ws), Var::abs_species(ws), basename,
             Var::verbosity(ws));
}

/*! Reads atmospheric data from a scenario.

An atmospheric scenario includes the following data for each
position (pressure, latitude, longitude) in the atmosphere:
   1. temperature field
   2. the corresponding altitude field
   3. vmr fields for the gaseous species
   4. Non-LTE temperature fields and matching identifiers
The data is stored in different files. This method reads all
files and creates the variables *t_field_raw*, *z_field_raw*,
*vmr_field_raw*, *nlte_field_raw*, and *nlte_level_identifiers*.

Files in a scenarios should be named matching the pattern of:
tropical.H2O.xml

The files can be anywhere, but they must be all in the same
directory, selected by 'basename'. The files are chosen by the
species name. If you have more than one tag group for the same
species, the same profile will be used.

@author Claudia Emde
@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] basename - Name of scenario, probably including the full path. For
example: "/smiles_local/arts-data/atmosphere/fascod/tropical"
@param[in] expect_vibrational_energies - Should ev.xml be read? (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void AtmWithNLTERawRead(Workspace& ws, const String& basename,
                        const Index& expect_vibrational_energies = 0) {
  AtmWithNLTERawRead(Var::t_field_raw(ws), Var::z_field_raw(ws),
                     Var::vmr_field_raw(ws), Var::nlte_field_raw(ws),
                     Var::nlte_level_identifiers(ws),
                     Var::nlte_vibrational_energies(ws), Var::abs_species(ws),
                     basename, expect_vibrational_energies, Var::verbosity(ws));
}

/*! Sets the atmospheric dimension to 1D.

Sets *atmosphere_dim* to 1, and the latitude and longitude grids
are set to be empty.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void AtmosphereSet1D(Workspace& ws) {
  AtmosphereSet1D(Var::atmosphere_dim(ws), Var::lat_grid(ws), Var::lon_grid(ws),
                  Var::verbosity(ws));
}

/*! Sets the atmospheric dimension to be 2D.

Sets *atmosphere_dim* to 2 and the longitude grid to be empty.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void AtmosphereSet2D(Workspace& ws) {
  AtmosphereSet2D(Var::atmosphere_dim(ws), Var::lon_grid(ws),
                  Var::verbosity(ws));
}

/*! Sets the atmospheric dimension to 3D.

Sets *atmosphere_dim* to 3, and *lat_true* and *lon_true* are
set to be empty.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void AtmosphereSet3D(Workspace& ws) {
  AtmosphereSet3D(Var::atmosphere_dim(ws), Var::lat_true(ws), Var::lon_true(ws),
                  Var::verbosity(ws));
}

/*! Display information about the given CIA tags.
The CIA tags shown are in the same format as needed by *abs_speciesSet*.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] catalogpath - Path to the CIA catalog directory.
@param[in] cia_tags - Array of CIA tags to view, e.g. [ "N2-N2", "H2-H2" ]

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void CIAInfo(Workspace& ws, const String& catalogpath,
             const ArrayOfString& cia_tags) {
  CIAInfo(catalogpath, cia_tags, Var::verbosity(ws));
}

/*! Creates a variable of group CIARecord.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void CIARecordCreate(Workspace& ws, CIARecord& out) {
  CIARecordCreate(out, Var::verbosity(ws));
}

/*! Reads CIARecord from Hitran-style file.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] cia_record - CIARecord type variable for input and output.
@param[in] species_tag - SpeciesTag string to associate with this CIARecord. See
*abs_speciesSet* for correct format.
@param[in] filename - Filename of HITRAN CIA data file.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void CIARecordReadFromFile(Workspace& ws, CIARecord& cia_record,
                           const String& species_tag, const String& filename) {
  CIARecordReadFromFile(cia_record, species_tag, filename, Var::verbosity(ws));
}

/*! Checks the consistency between two variables.

The two variables are checked to not deviate outside the specified
value (*maxabsdiff*). An error is issued if this is not fulfilled.

The main application of this method is to be part of the test
control files, and then used to check that a calculated value
is consistent with an old, reference, value.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] var1 - A first variable
@param[in] var2 - A second variable
@param[in] maxabsdiff - Threshold for maximum absolute difference. (default: {})
@param[in] error_message - Additional error message. (default: "")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0, typename Any_1>
void Compare(Workspace& ws, const std::pair<Any_0, String>& var1,
             const std::pair<Any_1, String>& var2,
             const std::pair<Numeric, String>& maxabsdiff = {{}, "maxabsdiff"},
             const std::pair<String, String>& error_message = {
                 "", "error_message"}) {
  Compare(var1.first, var2.first, maxabsdiff.first, error_message.first,
          var1.second, var2.second, maxabsdiff.second, error_message.second,
          Var::verbosity(ws));
}

/*! Checks the consistency between two variables by their relative values.

The two variables are checked to not deviate outside the specified
relative value (*maxabsreldiff*). An error is issued if this is not
fulfilled.

The main application of this method is to be part of the test
control files, and then used to check that a calculated value
is consistent with an old, reference, value.

If either value is 0.0, the relative error is considered as 0
for easier use.  This really means infinite differences, though
allowing zero-crossings is useful for plenty of tests. So Be Aware!

If both *var1* and *var2* are non-zero, the difference is evaluated
as: abs(var1/var2-1)
That is, *var2* is taken as the reference value.

@author Oliver Lemke
@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] var1 - A first variable
@param[in] var2 - A second variable
@param[in] maxabsreldiff - Threshold for maximum relative difference.
@param[in] error_message - Additional error message. (default: "")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0, typename Any_1>
void CompareRelative(Workspace& ws, const std::pair<Any_0, String>& var1,
                     const std::pair<Any_1, String>& var2,
                     const std::pair<Numeric, String>& maxabsreldiff,
                     const std::pair<String, String>& error_message = {
                         "", "error_message"}) {
  CompareRelative(var1.first, var2.first, maxabsreldiff.first,
                  error_message.first, var1.second, var2.second,
                  maxabsreldiff.second, error_message.second,
                  Var::verbosity(ws));
}

/*! Copy a workspace variable.

This method can copy any workspace variable
to another workspace variable of the same group. (E.g., a Matrix to
another Matrix.)

As always, output comes first in the argument list!

Usage example:

Copy(f_grid, p_grid)

Will copy the content of *p_grid* to *f_grid*. The size of *f_grid*
is adjusted automatically (the normal behaviour for workspace
methods).

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Destination variable.
@param[in] in - Source variable.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0, typename Any_1>
void Copy(Workspace& ws, std::pair<Any_0, String>& out,
          const std::pair<Any_1, String>& in) {
  Copy(out.first, out.second, in.first, in.second, Var::verbosity(ws));
}

/*! Creates a variable of group CovarianceMatrix.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void CovarianceMatrixCreate(Workspace& ws, CovarianceMatrix& out) {
  CovarianceMatrixCreate(out, Var::verbosity(ws));
}

/*! Sets the angular grids for Discrete Ordinate type scattering
calculations.

This method sets the angular grids for the Discrete Ordinate type
scattering calculations (DOIT, DISORT). For down- und up-looking
geometries it suffices to define *N_za_grid* (both solvers) and
*N_aa_grid* (DOIT). From these numbers equally spaced grids are
created and stored in the WSVs *za_grid* and *aa_grid*.

For limb simulations it is important to use an optimized zenith
angle grid with a very fine resolution around the horizon
(za=90 degrees). Such a grid can be generated using
*doit_za_grid_optCalc*. To be applied, the name of the file holding
the optimized angle grid has to be given (*za_grid_opt_file*).

When an optimized grid is present, the equidistant grid is used for
the calculation of the scattering integrals, while the optimized
grid is applied for the integration of the radiative transfer
equation. Otherwise the equidistant grid is used throughout. For
down-looking cases using the equidistant grid typically suffices
and speeds up the calculations.

@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace
@param[in] N_za_grid - Number of grid points in zenith angle grid. Recommended
value is 19.
@param[in] N_aa_grid - Number of grid points in azimuth angle grid. Recommended
value is 37. (default: 1)
@param[in] za_grid_opt_file - Name of special grid for RT part. (default: "")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void DOAngularGridsSet(Workspace& ws, const Index& N_za_grid,
                       const Index& N_aa_grid = 1,
                       const String& za_grid_opt_file = "") {
  DOAngularGridsSet(Var::doit_za_grid_size(ws), Var::aa_grid(ws),
                    Var::za_grid(ws), N_za_grid, N_aa_grid, za_grid_opt_file,
                    Var::verbosity(ws));
}

/*! Performs batch calculations for radiation fields.

We perform *ybatch_n* jobs, starting at index *ybatch_start*. (Zero
based indexing, as usual.) The output arrays will have
ybatch_n elements. Indices in the output array start
with zero, independent of *ybatch_start*.

WARNING, MEMORY INTENSIVE!!!: Since the outputs of this methods can
be very large, make sure you only pass back output you need.
Estimate the size of your output by looking at the dimensions
beforehand. If you only want to pass back some fields, make sure to
empty the others at the end of your *dobatch_calc_agenda*. E.g.:
Tensor7SetConstant(cloudbox_field, 0, 0, 0, 0, 0, 0, 0, 0.)

The method performs the following:
   1. Sets *ybatch_index* = *ybatch_start*.
   2. Performs a-d until
      *ybatch_index* = *ybatch_start* + *ybatch_n*.
        a. Executes *dobatch_calc_agenda*.
        b. If *ybatch_index* = *ybatch_start*, resizes the output
           arrays based on *ybatch_n*.
        c. Copies calculated fields to *ybatch_index* - *ybatch_start*
           of output arrays.
        d. Adds 1 to *ybatch_index*.

Beside the *dobatch_calc_agenda*, the WSVs *ybatch_start*
and *ybatch_n* must be set before calling this method.

The input variable *ybatch_start* is set to a default of zero in
*general.arts*.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] robust - A flag with value 1 or 0. If set to one, the batch
calculation will continue, even if individual jobs fail. In
that case, a warning message is written to screen and file
(out1 output stream), and the output array entry for the
failed job in the output fields is left empty. (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void DOBatchCalc(Workspace& ws, const Index& robust = 0) {
  DOBatchCalc(ws, Var::dobatch_cloudbox_field(ws),
              Var::dobatch_radiance_field(ws),
              Var::dobatch_irradiance_field(ws),
              Var::dobatch_spectral_irradiance_field(ws), Var::ybatch_start(ws),
              Var::ybatch_n(ws), Var::dobatch_calc_agenda(ws), robust,
              Var::verbosity(ws));
}

/*! Deletes a workspace variable.

The variable is marked as uninitialized and its memory freed.
It is not removed from the workspace though, therefore you
don't need to/can't call Create for this variable again.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] v - Variable to be deleted.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void Delete(Workspace& ws, const std::pair<Any_0, String>& v) {
  Delete(ws, v.first, v.second, Var::verbosity(ws));
}

/*! Create a diagonal matrix from a vector.
This creates a dense or sparse diagonal matrix with the elements of the given
vector on the diagonal.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - The diagonal matrix
@param[in] v - The vector containing the diagonal elements.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void DiagonalMatrix(Workspace& ws, Any_0& out, const Vector& v) {
  DiagonalMatrix(out, v, Var::verbosity(ws));
}

/*! Derives the difference betwenn zenith and azimuth angles.

Determines the difference between a set of angles (*other_los*)
and a reference direction (*ref_los*). This method reverses the
addition made by *AddZaAa*.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] dlos - Derived differences in line-of-sight.
@param[in] ref_los - Reference line-of-sight (a single LOS).
@param[in] other_los - Other line-of-sights (can be multiple LOS).

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void DiffZaAa(Workspace& ws, Matrix& dlos, const Vector& ref_los,
              const Matrix& other_los) {
  DiffZaAa(dlos, ref_los, other_los, Var::verbosity(ws));
}

/*! Interface to the DISORT scattering solver (by Stamnes et al.).

DISCLAIMER: There is a couple of known issues with the current
implementation (see below). Use this WSM with care and only if
these limitations/requirements are fulfilled. Results might be
erroneous otherwise.

DISORT provides the radiation field (*cloudbox_field*) from a scalar
1D scattering solution assuming a plane-parallel atmosphere (flat
Earth). Only totally randomly oriented particles are allowed.
Refraction is not taken into account. Only Lambertian surface
reflection is handled.

*nstreams* is the number of polar angles taken into account
internally in the scattering solution, *za_grid* is the
polar angle grid on which *cloudbox_field* is provided.
*nstreams* determines the angular resolution, hence the accuracy,
of the scattering solution. The more anisotropic the bulk scattering
matrix, the more streams are required. The computational burden
increases approximately linearly with *nstreams*. The default value
(8) is sufficient for most microwave scattering calculations. It is
likely insufficient for IR calculations involving ice clouds,
though.

Further, *za_grid* determines the resolution of the output
radiation field. The size of *za_grid* has no practical
impact on computation time in the case of Disort and higher
resolution generally improves the interpolation results, hence
larger *za_grid* are recommended. To ensure sufficient
interpolation accuracy, we require a (hardcoded) minimum size of 38.

Different sphericity levels are emulated here by embedding DISORT
in different ways and using different output. The available options
(from low to high sphericity level) are:
- Cloudbox extends over whole atmosphere (e.g. by setting cloudbox
  from *cloudboxSetFullAtm*).
- Cloudbox extends over a limited part of the atmosphere only (e.g.
  by setting cloudbox from *cloudboxSetAutomatically* or
  *cloudboxSetManually*). Internally, DISORT is run over the whole
  atmosphere, but only the radiation field within the cloudbox is
  passed on and used further in ARTS (e.g. by *yCalc*).

Known issues of ARTS implementation:
- Surface altitude is not an interface parameter. Surface is
  implicitly assumed to be at the lowest atmospheric level.
- Scattering angle grids of all scattering elements have to be
  identical (except if *pfct_method* is 'interpolate').

Keyword *pfct_method* allows to chose the method to extract phase
function. 'interpolate' considers temperature dependence. Others
neglect it by chosing one specific temperature grid point from the
single scattering data: 'low' choses the lowest T-point, 'high' the
highest T-point, and 'median' the median T-point. As different
scattering elements can have different temperature grids, the actual
temperature value used can differ between the scattering elements.
Currently, other methods than 'interpolate' require all scattering
elements to be given on identical scattering angle grids.
Note that this keyword solely affects the phase function;
extinction/absorption/scattering cross sections are always
interpolated to the actual temperature.

@author Claudia Emde, Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] nstreams - Number of polar angle directions (streams) in DISORT
solution (must be an even number). (default: 8)
@param[in] pfct_method - Flag which method to apply to derive phase function.
(default: "median")
@param[in] Npfct - Number of angular grid points to calculate bulk phase
function on (and derive Legendre polnomials from). If <0, the finest za_grid
from scat_data will be used. (default: 181)
@param[in] quiet - Silence C Disort warnings. (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void DisortCalc(Workspace& ws, const Index& nstreams = 8,
                const String& pfct_method = "median", const Index& Npfct = 181,
                const Index& quiet = 0) {
  DisortCalc(ws, Var::cloudbox_field(ws), Var::atmfields_checked(ws),
             Var::atmgeom_checked(ws), Var::scat_data_checked(ws),
             Var::cloudbox_checked(ws), Var::cloudbox_on(ws),
             Var::cloudbox_limits(ws), Var::propmat_clearsky_agenda(ws),
             Var::atmosphere_dim(ws), Var::pnd_field(ws), Var::t_field(ws),
             Var::z_field(ws), Var::vmr_field(ws), Var::p_grid(ws),
             Var::scat_data(ws), Var::f_grid(ws), Var::za_grid(ws),
             Var::stokes_dim(ws), Var::z_surface(ws), Var::surface_skin_t(ws),
             Var::surface_scalar_reflectivity(ws), nstreams, pfct_method, Npfct,
             quiet, Var::verbosity(ws));
}

/*! Interface to DISORT for running clear-sky cases.

The method runs DISORT with *pnd_field* set to zero.

Note that this version returns *spectral_radiance_field*, i.e.
the solution for the full atmosphere. The standard *DisortCalc*
only returns the field inside the cloudbox.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] nstreams - Number of polar angle directions (streams) in DISORT
solution (must be an even number). (default: 8)
@param[in] quiet - Silence C Disort warnings. (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void DisortCalcClearsky(Workspace& ws, const Index& nstreams = 8,
                        const Index& quiet = 0) {
  DisortCalcClearsky(
      ws, Var::spectral_radiance_field(ws), Var::atmfields_checked(ws),
      Var::atmgeom_checked(ws), Var::propmat_clearsky_agenda(ws),
      Var::atmosphere_dim(ws), Var::t_field(ws), Var::z_field(ws),
      Var::vmr_field(ws), Var::p_grid(ws), Var::f_grid(ws), Var::za_grid(ws),
      Var::stokes_dim(ws), Var::z_surface(ws), Var::surface_skin_t(ws),
      Var::surface_scalar_reflectivity(ws), nstreams, quiet,
      Var::verbosity(ws));
}

/*! DISORT with surface.

@author Claudia Emde, Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] nstreams - Number of polar angle directions (streams) in DISORT
solution (must be an even number). (default: 8)
@param[in] pfct_method - Flag which method to apply to derive phase function.
(default: "median")
@param[in] Npfct - Number of angular grid points to calculate bulk phase
function on (and derive Legendre polnomials from). If <0, the finest za_grid
from scat_data will be used. (default: 181)
@param[in] quiet - Silence C Disort warnings. (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void DisortCalcWithARTSSurface(Workspace& ws, const Index& nstreams = 8,
                               const String& pfct_method = "median",
                               const Index& Npfct = 181,
                               const Index& quiet = 0) {
  DisortCalcWithARTSSurface(
      ws, Var::cloudbox_field(ws), Var::atmfields_checked(ws),
      Var::atmgeom_checked(ws), Var::scat_data_checked(ws),
      Var::cloudbox_checked(ws), Var::cloudbox_on(ws), Var::cloudbox_limits(ws),
      Var::propmat_clearsky_agenda(ws), Var::surface_rtprop_agenda(ws),
      Var::atmosphere_dim(ws), Var::pnd_field(ws), Var::t_field(ws),
      Var::z_field(ws), Var::vmr_field(ws), Var::p_grid(ws), Var::scat_data(ws),
      Var::f_grid(ws), Var::za_grid(ws), Var::stokes_dim(ws), nstreams,
      pfct_method, Npfct, quiet, Var::verbosity(ws));
}

/*! Main DOIT method.

This method executes *doit_mono_agenda* for each frequency
in *f_grid*. The output is the radiation field inside the cloudbox
(*cloudbox_field*).

@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void DoitCalc(Workspace& ws) {
  DoitCalc(ws, Var::cloudbox_field(ws), Var::atmfields_checked(ws),
           Var::atmgeom_checked(ws), Var::cloudbox_checked(ws),
           Var::scat_data_checked(ws), Var::cloudbox_on(ws), Var::f_grid(ws),
           Var::doit_mono_agenda(ws), Var::doit_is_initialized(ws),
           Var::verbosity(ws));
}

/*! Calculates incoming radiation field of the cloudbox by repeated
radiative transfer calculations.

The method performs monochromatic pencil beam calculations for
all grid positions on the cloudbox boundary, and all directions
given by scattering angle grids (*scat_za/aa_grid*). Found radiances
are stored in *cloudbox_field* which can be used as boundary
conditions when scattering inside the cloud box is solved by the
*DoitCalc* method.

Note that *cloudbox_field* will always hold intensity in terms of
radiances, regardless of the setting of *iy_unit* (unit conversion
is done within *yCalc* or *iyCalc*, which will provide their output
in terms of the specified *iy_unit*; no explicit unit conversion by
the user necessary.).

@author Sreerekha T.R.
@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace
@param[in] rigorous - Fail if incoming field is not safely interpolable.
(default: 1)
@param[in] maxratio - Maximum allowed ratio of two radiances regarded as
interpolable. (default: 100)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void DoitGetIncoming(Workspace& ws, const Index& rigorous = 1,
                     const Numeric& maxratio = 100) {
  DoitGetIncoming(ws, Var::cloudbox_field(ws), Var::atmfields_checked(ws),
                  Var::atmgeom_checked(ws), Var::cloudbox_checked(ws),
                  Var::doit_is_initialized(ws), Var::iy_main_agenda(ws),
                  Var::atmosphere_dim(ws), Var::lat_grid(ws), Var::lon_grid(ws),
                  Var::z_field(ws), Var::nlte_field(ws), Var::cloudbox_on(ws),
                  Var::cloudbox_limits(ws), Var::f_grid(ws),
                  Var::stokes_dim(ws), Var::za_grid(ws), Var::aa_grid(ws),
                  rigorous, maxratio, Var::verbosity(ws));
}

/*! As *DoitGetIncoming* but assumes clear sky part to be 1D.
The incoming field is calculated only for one position and azimuth
angle for each cloud box boundary, and obtained values are used
for all other postions and azimuth angles. This works if a 3D
cloud box is put into an 1D background atmosphere.

This method can only be used for 3D cases.

@author Sreerekha T.R.
@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void DoitGetIncoming1DAtm(Workspace& ws) {
  DoitGetIncoming1DAtm(ws, Var::cloudbox_field(ws), Var::cloudbox_on(ws),
                       Var::atmfields_checked(ws), Var::atmgeom_checked(ws),
                       Var::cloudbox_checked(ws), Var::doit_is_initialized(ws),
                       Var::iy_main_agenda(ws), Var::atmosphere_dim(ws),
                       Var::lat_grid(ws), Var::lon_grid(ws), Var::z_field(ws),
                       Var::nlte_field(ws), Var::cloudbox_limits(ws),
                       Var::f_grid(ws), Var::stokes_dim(ws), Var::za_grid(ws),
                       Var::aa_grid(ws), Var::verbosity(ws));
}

/*! Initialises variables for DOIT scattering calculations.

Note that multi-dimensional output variables (Tensors, specifically)
are NaN-initialized. That is, this methods needs to be called
BEFORE other WSMs that provide input to *DoitCalc*, e.g. before
*DoitGetIncoming*.

@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void DoitInit(Workspace& ws) {
  DoitInit(Var::doit_scat_field(ws), Var::cloudbox_field(ws),
           Var::doit_is_initialized(ws), Var::stokes_dim(ws),
           Var::atmosphere_dim(ws), Var::f_grid(ws), Var::za_grid(ws),
           Var::aa_grid(ws), Var::doit_za_grid_size(ws), Var::cloudbox_on(ws),
           Var::cloudbox_limits(ws), Var::verbosity(ws));
}

/*! Prepares single scattering data for a DOIT scattering calculation.

First the scattering data is interpolated in frequency using
*scat_data_monoCalc*. Then the phase matrix data is
transformed or interpolated from the raw data to the laboratory frame
for all possible combinations of the angles contained in the angular
grids which are set in *DOAngularGridsSet*. The resulting phase
matrices are stored in *pha_mat_sptDOITOpt*.

@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void DoitScatteringDataPrepare(Workspace& ws) {
  DoitScatteringDataPrepare(
      ws, Var::pha_mat_sptDOITOpt(ws), Var::scat_data_mono(ws),
      Var::pha_mat_doit(ws), Var::aa_grid(ws), Var::doit_za_grid_size(ws),
      Var::scat_data(ws), Var::scat_data_checked(ws), Var::f_index(ws),
      Var::atmosphere_dim(ws), Var::stokes_dim(ws), Var::t_field(ws),
      Var::cloudbox_limits(ws), Var::pnd_field(ws), Var::pha_mat_spt_agenda(ws),
      Var::verbosity(ws));
}

/*! Writes DOIT iteration fields.

This method writes intermediate iteration fields to xml-files. The
method can be used as a part of *doit_conv_test_agenda*.

The iterations to be stored are specified by *iterations*, e.g.:
    iterations = [3, 6, 9]
In this case the 3rd, 6th and 9th iterations are stored.
If a number is larger than the total number of iterations, this
number is ignored. If all iterations should be stored set
   iterations = [-1]

The frequencies to be stored are specified by *frequencies* in the
same way as the iterations. The frequency index corresponds to the
order of frequencies in *f_grid*.

The output files are named doit_iteration_fX_iY.xml with X being the
frequency index and iY the iteration counter.

@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace
@param[in] iterations - Selection of iterations to store. (default: {-1})
@param[in] frequencies - Selection of frequencies to store. (default: {-1})

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void DoitWriteIterationFields(Workspace& ws,
                              const ArrayOfIndex& iterations = {-1},
                              const ArrayOfIndex& frequencies = {-1}) {
  DoitWriteIterationFields(Var::doit_iteration_counter(ws),
                           Var::cloudbox_field_mono(ws), Var::f_index(ws),
                           iterations, frequencies, Var::verbosity(ws));
}

/*! Sets the seconds between two times.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] duration - Time in seconds between *start* and *end*
@param[in] start - Start time
@param[in] end - End time

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Duration(Workspace& ws, Numeric& duration, const Time& start,
              const Time& end) {
  Duration(duration, start, end, Var::verbosity(ws));
}

/*! Creates a variable of group EnergyLevelMap.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void EnergyLevelMapCreate(Workspace& ws, EnergyLevelMap& out) {
  EnergyLevelMapCreate(out, Var::verbosity(ws));
}

/*! Sets an EnergyLevelMap

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] x - out
@param[in] y - in

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void EnergyLevelMapSet(Workspace& ws, EnergyLevelMap& x,
                       const EnergyLevelMap& y) {
  EnergyLevelMapSet(x, y, Var::verbosity(ws));
}

/*! Issues an error and exits ARTS.

This method can be placed in agendas that must be specified, but
are expected not to be used for the particular case. An inclusion
in *surface_rtprop_agenda* could look like:
   Error{"Surface interceptions of propagation path not expected."}

Ignore and other dummy method calls must still be included.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] msg - String describing the error.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Error(Workspace& ws, const String& msg) { Error(msg, Var::verbosity(ws)); }

/*! Stops the execution and exits ARTS.

This method is handy if you want to debug one of your control
files. You can insert it anywhere in the control file. When
it is reached, it will terminate the program.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Exit(Workspace& ws) { Exit(Var::verbosity(ws)); }

/*! Extracts an element from an array.

Copies the element with the given Index from the input
variable to the output variable.

For a Tensor3 as an input, it copies the page with the given
Index from the input Tensor3 variable to the output Matrix.

In other words, the selection is always done on the first dimension.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] needle - Extracted element.
@param[in] haystack - Variable to extract from.
@param[in] index - Position of the element which should be extracted.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0, typename Any_1>
void Extract(Workspace& ws, Any_0& needle, const Any_1& haystack,
             const Index& index) {
  Extract(needle, haystack, index, Var::verbosity(ws));
}

/*! Extract (numeric) parameters from scat_meta of a single scattering
species.

...

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[out] meta_param - The extracted meta parameter values.
@param[in] meta_name - Name of the meta parameter to extract.
@param[in] scat_species_index - Array index of scattering species from which to
extract.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ExtractFromMetaSingleScatSpecies(Workspace& ws, Vector& meta_param,
                                      const String& meta_name,
                                      const Index& scat_species_index) {
  ExtractFromMetaSingleScatSpecies(meta_param, Var::scat_meta(ws), meta_name,
                                   scat_species_index, Var::verbosity(ws));
}

/*! Stand-alone usage of FASTEM.

FASTEM is a parameterisation of the emissivity of water surfaces
including the impact of waves, salinity and non-specular effects.
This is more or less direct interface to FASTEM, but slightly
adopted to fit with ARTS. The unit of frequency and salinity
differ, and this version is "vectorised" in frequency.

The output is four emissivity and reflectivity values for each
frequency. These values are defined in Eq. 13 of  "An Improved
Fast Microwave Water Emissivity Model" by Liu, Weng and English,
I3TRGS, 2011. Note that emissivity and reflectivity do not add up
to 1, which is the way FASTEM compensates for non-specular effects.

There is an error if any frequency is above 250 GHz, or if the skin
temperature is below 260 K. If the skin temperature is below 270 K,
it is adjusted to 270 K.

FASTEM returns unphysical values for propagation close to the
horizon, here emissivity and reflectivity can be outside [0,1].
If either emissivity or reflectivity is below/above 0/1, it is
set to 0/1, and the other value is set to 1/0. That is, e+r=1
is enforced. These problems start about 15 degrees from the horizon.

@author Oliver Lemke, Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] emissivity - Emission values. One row for each frequency. See above.
@param[out] reflectivity - Reflectivity values. One row for each frequency. See
above.
@param[in] za - Zenith angle of line-of-sigh, 90 to 180 deg.
@param[in] salinity - Salinity, 0-1. That is, 3% is given as 0.03. (default:
0.035)
@param[in] wind_speed - Wind speed.
@param[in] rel_aa - Azimuth angle between wind direction and line-of-sight. This
angle is measured clockwise from north, i.e. E=90deg.
@param[in] transmittance - The transmission of the atmosphere, along the
propagation path of the downwelling radiation. One value per frequency.
@param[in] fastem_version - The version of FASTEM to use. (default: 6)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void FastemStandAlone(Workspace& ws, Matrix& emissivity, Matrix& reflectivity,
                      const Numeric& za, const Numeric& wind_speed,
                      const Numeric& rel_aa, const Vector& transmittance,
                      const Numeric& salinity = 0.035,
                      const Index& fastem_version = 6) {
  FastemStandAlone(emissivity, reflectivity, Var::f_grid(ws),
                   Var::surface_skin_t(ws), za, salinity, wind_speed, rel_aa,
                   transmittance, fastem_version, Var::verbosity(ws));
}

/*! Extract the data from a GriddedField.

A check is performed that the grids from the
GriddedField match *p_grid*, *lat_grid* and *lon_grid*.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Extracted field.
@param[in] in - Raw input gridded field.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0, typename Any_1>
void FieldFromGriddedField(Workspace& ws, Any_0& out, const Any_1& in) {
  FieldFromGriddedField(out, Var::p_grid(ws), Var::lat_grid(ws),
                        Var::lon_grid(ws), in, Var::verbosity(ws));
}

/*! Sets an index variable that acts as an on/off flag to 0.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] flag - Variable to set to 0.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void FlagOff(Workspace& ws, Index& flag) { FlagOff(flag, Var::verbosity(ws)); }

/*! Sets an index variable that acts as an on/off flag to 1.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] flag - Variable to set to 1.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void FlagOn(Workspace& ws, Index& flag) { FlagOn(flag, Var::verbosity(ws)); }

/*! Flattens an ArrayOfArray<T> to Array<T> or an Array
of matpack-types to a larger dimension matpack (if dimensions agree)

The intended transformation for arrays is (sub-arrays can have different sizes):
    {{a, b, c}, {d, e}} -> {a, b, c, d, e}

The intended transformation for arrays to matpack types is (sub-types must have
same size):
    {{a, b, c}, {d, e, f}} -> {a, b, c, d, e, f}

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Flatter array/matpack-type
@param[in] in - An array

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0, typename Any_1>
void Flatten(Workspace& ws, Any_0& out, const Any_1& in) {
  Flatten(out, in, Var::verbosity(ws));
}

/*! A simple for-loop.

This method is handy when you quickly want to test out a calculation
with a set of different settings.

It does a for-loop from start to stop in steps of step (who would
have guessed that). For each iteration, the agenda *forloop_agenda* is
executed. Inside the agenda, the variable *forloop_index* is available
as index counter.

There are no other inputs to *forloop_agenda*, and also no outputs. That
means, if you want to get any results out of this loop, you have to
save it to files (for example with *WriteXMLIndexed*), since
variables used inside the agenda will only be local.

Note that this kind of for loop is not parallel.

The method is intended for simple testing, not as a replacement of
*ybatchCalc*. However, it is compatible with *ybatchCalc*, in the sense
that *ybatchCalc* may occur inside *forloop_agenda*.

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace
@param[in] start - Start value.
@param[in] stop - End value.
@param[in] step - Step size.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ForLoop(Workspace& ws, const Index& start, const Index& stop,
             const Index& step) {
  ForLoop(ws, Var::forloop_agenda(ws), start, stop, step, Var::verbosity(ws));
}

/*! Convert from angular wavenumber [cm^-1] to frequency [Hz].

This converts angular wavenumber (2*PI/wavelength) into frequency.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] frequency - frequency [Hz]
@param[in] angular_wavenumber - angular wavenumber [cm^-1]

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0, typename Any_1>
void FrequencyFromCGSAngularWavenumber(Workspace& ws, Any_0& frequency,
                                       const Any_1& angular_wavenumber) {
  FrequencyFromCGSAngularWavenumber(frequency, angular_wavenumber,
                                    Var::verbosity(ws));
}

/*! Convert from Kayser wavenumber [cm^-1] to frequency [Hz].

This converts Kayser wavenumber (1/wavelength) into frequency.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] frequency - frequency [Hz]
@param[in] kayser_wavenumber - Kayser wavenumber [cm^-1]

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0, typename Any_1>
void FrequencyFromCGSKayserWavenumber(Workspace& ws, Any_0& frequency,
                                      const Any_1& kayser_wavenumber) {
  FrequencyFromCGSKayserWavenumber(frequency, kayser_wavenumber,
                                   Var::verbosity(ws));
}

/*! Convert from wavelength [m] to frequency [Hz].

This is a generic method. It can take a single wavelength value or a wavelength
vector as input.

@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace
@param[out] frequency - frequency [Hz]
@param[in] wavelength - wavelength [m]

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0, typename Any_1>
void FrequencyFromWavelength(Workspace& ws, Any_0& frequency,
                             const Any_1& wavelength) {
  FrequencyFromWavelength(frequency, wavelength, Var::verbosity(ws));
}

/*! Creates a variable of group GasAbsLookup.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void GasAbsLookupCreate(Workspace& ws, GasAbsLookup& out) {
  GasAbsLookupCreate(out, Var::verbosity(ws));
}

/*! Copy the contents of an environment variable to an ARTS String or Index.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Contents of environment variable.
@param[in] in - Name of environment variable.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void GetEnvironmentVariable(Workspace& ws, Any_0& out, const String& in) {
  GetEnvironmentVariable(out, in, Var::verbosity(ws));
}

/*! Returns the number of threads used by ARTS.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] nthreads - Number of threads.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void GetNumberOfThreads(Workspace& ws, Index& nthreads) {
  GetNumberOfThreads(nthreads, Var::verbosity(ws));
}

/*! Creates a variable of group GridPos.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void GridPosCreate(Workspace& ws, GridPos& out) {
  GridPosCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group GriddedField1.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void GriddedField1Create(Workspace& ws, GriddedField1& out) {
  GriddedField1Create(out, Var::verbosity(ws));
}

/*! Creates a variable of group GriddedField2.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void GriddedField2Create(Workspace& ws, GriddedField2& out) {
  GriddedField2Create(out, Var::verbosity(ws));
}

/*! Creates a variable of group GriddedField3.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void GriddedField3Create(Workspace& ws, GriddedField3& out) {
  GriddedField3Create(out, Var::verbosity(ws));
}

/*! Creates a variable of group GriddedField4.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void GriddedField4Create(Workspace& ws, GriddedField4& out) {
  GriddedField4Create(out, Var::verbosity(ws));
}

/*! Creates a variable of group GriddedField5.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void GriddedField5Create(Workspace& ws, GriddedField5& out) {
  GriddedField5Create(out, Var::verbosity(ws));
}

/*! Creates a variable of group GriddedField6.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void GriddedField6Create(Workspace& ws, GriddedField6& out) {
  GriddedField6Create(out, Var::verbosity(ws));
}

/*! Get the name of a GriddedField.

See *ArrayOfGriddedFieldGetNames*.

@author Lukas Kluft

@param[in,out] Workspace ws - An ARTS workspace
@param[out] name - Name of the GriddedField.
@param[in] griddedfield - GriddedField.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void GriddedFieldGetName(Workspace& ws, String& name,
                         const Any_0& griddedfield) {
  GriddedFieldGetName(name, griddedfield, Var::verbosity(ws));
}

/*! Expands the latitude and longitude grid of the GriddedField to
[-90, 90] and [0,360], respectively. Expansion is only done in
the dimension(s), where the grid size is 1.
The values from the input data will be duplicated to accomodate
for the larger size of the output field.
gfield_raw_out and gfield_raw_in can be the same variable.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Expanded gridded field.
@param[in] in - Raw input gridded field.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0, typename Any_1>
void GriddedFieldLatLonExpand(Workspace& ws, Any_0& out, const Any_1& in) {
  GriddedFieldLatLonExpand(out, in, Var::verbosity(ws));
}

/*! Interpolates the input field along the latitude and longitude dimensions
to *lat_true* and *lon_true*.

If the input longitude grid is outside of *lon_true* it will be shifted
left or right by 360. If it covers 360 degrees, a cyclic interpolation
will be performed.
in and out fields can be the same variable.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Regridded gridded field.
@param[in] in - Raw input gridded field.
@param[in] interp_order - Interpolation order. (default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0, typename Any_1>
void GriddedFieldLatLonRegrid(Workspace& ws, Any_0& out, const Any_1& in,
                              const Index& interp_order = 1) {
  GriddedFieldLatLonRegrid(out, Var::lat_true(ws), Var::lon_true(ws), in,
                           interp_order, Var::verbosity(ws));
}

/*! Interpolates the input field along the pressure dimension to *p_grid*.

If zero-padding is applied (zeropadding=1), pressures that are
outside the *p_grid* are set to 0. This is thought, e.g., for VMR
fields that outside the given pressure can safely be assumed to be
zero.
Note: Using zeropadding for altitude and temperature fields is
strongly discouraged (it will work here, though, but likely trigger
errors later on).
Extrapolation is allowed within the common 0.5grid-step margin,
but is overruled by zeropadding.
in and out fields can be the same variable.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Regridded gridded field.
@param[in] in - Raw input gridded field.
@param[in] interp_order - Interpolation order. (default: 1)
@param[in] zeropadding - Apply zero-padding. (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0, typename Any_1>
void GriddedFieldPRegrid(Workspace& ws, Any_0& out, const Any_1& in,
                         const Index& interp_order = 1,
                         const Index& zeropadding = 0) {
  GriddedFieldPRegrid(out, Var::p_grid(ws), in, interp_order, zeropadding,
                      Var::verbosity(ws));
}

/*! Interpolates the input field along the vertical dimension to *p_grid*.

This is done from z_field, and thus requires the atmosphere to be set
beforehand.

The latitude and longitude grid of the input field must match *lat_grid*
and *lon_grid* for the method to work.

BETA mode.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Regridded output; Pressure-gridded field.
@param[in] in - Raw input; Altitude-gridded field.
@param[in] interp_order - Interpolation order. (default: 1)
@param[in] zeropadding - Apply zero-padding. (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void GriddedFieldZToPRegrid(Workspace& ws, GriddedField3& out,
                            const GriddedField3& in,
                            const Index& interp_order = 1,
                            const Index& zeropadding = 0) {
  GriddedFieldZToPRegrid(out, Var::p_grid(ws), Var::lat_grid(ws),
                         Var::lon_grid(ws), Var::z_field(ws), in, interp_order,
                         zeropadding, Var::verbosity(ws));
}

/*! Creates a variable of group HitranRelaxationMatrixData.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void HitranRelaxationMatrixDataCreate(Workspace& ws,
                                      HitranRelaxationMatrixData& out) {
  HitranRelaxationMatrixDataCreate(out, Var::verbosity(ws));
}

/*! Includes the contents of another controlfile.

The INCLUDE statement inserts the contents of the controlfile
with the given name into the current controlfile.
If the filename is given without path information, ARTS will
first search for the file in all directories specified with the
-I (see arts -h) commandline option and then in directories given
in the environment variable ARTS_INCLUDE_PATH. In the environment
variable multiple paths have to be separated by colons.

Note that INCLUDE is not a workspace method and thus the
syntax is different:

Arts {
  INCLUDE "general.arts"
}

Includes can also be nested. In the example above general.arts
can contain further includes which will then be treated
the same way.

The idea behind this mechanism is that you can write common settings
for a bunch of calculations into one file. Then, you can create
several controlfiles which include the basic settings and tweak them
for different cases. When you decide to make changes to your setup
that should apply to all calculations, you only have to make a
single change in the include file instead of modifying all your
controlfiles.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void INCLUDE(Workspace& ws) { INCLUDE(Var::verbosity(ws)); }

/*! Ignore a workspace variable.

This method is handy for use in agendas in order to suppress warnings
about unused input workspace variables. What it does is: Nothing!
In other words, it just ignores the variable it is called on.

This method can ignore any workspace variable you want.

Usage example:

AgendaSet(els_agenda){
  Ignore(ls_sigma)
  elsLorentz
}

Without Ignore you would get an error message, because 'els_agenda' is
supposed to use the Doppler width 'ls_sigma', but the Lorentz lineshape
'elsLorentz' does not need it.

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace
@param[in] in - Variable to be ignored.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void Ignore(Workspace& ws, const Any_0& in) {
  Ignore(in, Var::verbosity(ws));
}

/*! Adds a index and a value (out = in+value).

The result can either be stored in the same or another index.
(in and out can be the same variable, but not out and value)

@author Patrick Eriksson, Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Output numeric.
@param[in] in - Input Index.
@param[in] value - Value to add.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void IndexAdd(Workspace& ws, Index& out, const Index& in, const Index& value) {
  IndexAdd(out, in, value, Var::verbosity(ws));
}

/*! Creates a variable of group Index.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void IndexCreate(Workspace& ws, Index& out) {
  IndexCreate(out, Var::verbosity(ws));
}

/*! Counts number of points in the atmosphere.

For a 3D atmosphere the method sets *n* to:
  p_grid.nelem()*lat_grid.nelem()*lon_grid.nelem()
For 1D and 2D the same calculation is done, but ignoring dimensions
not active.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] n - Variable to set with number of points.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void IndexNumberOfAtmosphericPoints(Workspace& ws, Index& n) {
  IndexNumberOfAtmosphericPoints(n, Var::atmosphere_dim(ws), Var::p_grid(ws),
                                 Var::lat_grid(ws), Var::lon_grid(ws),
                                 Var::verbosity(ws));
}

/*! Sets an index workspace variable to the given value.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to initialize.
@param[in] value - Value.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void IndexSet(Workspace& ws, Index& out, const Index& value) {
  IndexSet(out, value, Var::verbosity(ws));
}

/*! Set an Index to point towards last position of array-type variables.

This method works as nelemGet, but gives the index number of the last
element (which equals nelem-1).

@author Patrick Eriksson
@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] v - The method is defined for these groups.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void IndexSetToLast(Workspace& ws, const Any_0& v) {
  IndexSetToLast(Var::nelem(ws), v, Var::verbosity(ws));
}

/*! Performas: out = in - 1

Input and output can be same variable.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Output index variable.
@param[in] in - Input index variable.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void IndexStepDown(Workspace& ws, Index& out, const Index& in) {
  IndexStepDown(out, in, Var::verbosity(ws));
}

/*! Performas: out = in + 1

Input and output can be same variable.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Output index variable.
@param[in] in - Input index variable.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void IndexStepUp(Workspace& ws, Index& out, const Index& in) {
  IndexStepUp(out, in, Var::verbosity(ws));
}

/*! Point interpolation of atmospheric fields.

The default way to specify the position is by *rtp_pos*.

Linear interpolation is applied.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Value obtained by the interpolation.
@param[in] field - Field to interpolate.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void InterpAtmFieldToPosition(Workspace& ws, Numeric& out,
                              const Tensor3& field) {
  InterpAtmFieldToPosition(out, Var::atmosphere_dim(ws), Var::p_grid(ws),
                           Var::lat_grid(ws), Var::lon_grid(ws),
                           Var::z_field(ws), Var::rtp_pos(ws), field,
                           Var::verbosity(ws));
}

/*! Latitude and longitude interpolation of a GriddedField2.

The default way to specify the position is by *rtp_pos*.

The interpolation is done for the latitude and longitude in
*rtp_pos*. The altitude in *rtp_pos* is completely ignored.
Linear interpolation is applied.

The input field (*gfield2*) is expected to have latitude and
longitude as first and second dimension.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Value obtained by interpolation.
@param[in] gfield2 - Gridded field to interpolate.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void InterpGriddedField2ToPosition(Workspace& ws, Numeric& out,
                                   const GriddedField2& gfield2) {
  InterpGriddedField2ToPosition(out, Var::atmosphere_dim(ws), Var::lat_grid(ws),
                                Var::lat_true(ws), Var::lon_true(ws),
                                Var::rtp_pos(ws), gfield2, Var::verbosity(ws));
}

/*! Point interpolation of surface fields.

The default way to specify the position is by *rtp_pos*.

Linear interpolation is applied.

The interpolation is done for the latitude and longitude in
*rtp_pos*, while the altitude in *rtp_pos* is not part of the
calculations. However, it is checked that the altitude of *rtp_pos*
is inside the range covered by *z_surface* with a 1 m margin, to
give a warning when the specified position is not consistent with
the surface altitudes.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Value obtained by interpolation.
@param[in] field - Field to interpolate.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void InterpSurfaceFieldToPosition(Workspace& ws, Numeric& out,
                                  const Matrix& field) {
  InterpSurfaceFieldToPosition(out, Var::atmosphere_dim(ws), Var::lat_grid(ws),
                               Var::lon_grid(ws), Var::rtp_pos(ws),
                               Var::z_surface(ws), field, Var::verbosity(ws));
}

/*! Sets the seconds between localtime and gmtime representation of now().

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] dt - Time in seconds between local and gmt

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void LocalTimeOffset(Workspace& ws, Numeric& dt) {
  LocalTimeOffset(dt, Var::verbosity(ws));
}

/*! Creates a variable of group MCAntenna.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void MCAntennaCreate(Workspace& ws, MCAntenna& out) {
  MCAntennaCreate(out, Var::verbosity(ws));
}

/*! A generalised 3D reversed Monte Carlo radiative algorithm, that
allows for 2D antenna patterns, surface reflection and arbitrary
sensor positions.

The main output variables *y* and *mc_error* represent the
Stokes vector integrated over the antenna function, and the
estimated error in this vector, respectively.

The WSV *mc_max_iter* describes the maximum number of `photons'
used in the simulation (more photons means smaller *mc_error*).
*mc_std_err* is the desired value of mc_error. *mc_max_time* is
the maximum allowed number of seconds for MCGeneral. The method
will terminate once any of the max_iter, std_err, max_time
criteria are met. If negative values are given for these
parameters then it is ignored.

The WSV *mc_min_iter* sets the minimum number of photons to apply
before the condition set by *mc_std_err* is considered. Values
of *mc_min_iter* below 100 are not accepted.

Only "1" and "RJBT" are allowed for *iy_unit*. The value of
*mc_error* follows the selection for *iy_unit* (both for in- and
output.

@author Cory Davis

@param[in,out] Workspace ws - An ARTS workspace
@param[in] l_mc_scat_order - The length to be given to *mc_scat_order*. Note
that scattering orders equal and above this value will not be counted. (default:
11)
@param[in] t_interp_order - Interpolation order of temperature for scattering
data (so far only applied in phase matrix, not in extinction and absorption.
(default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void MCGeneral(Workspace& ws, const Index& l_mc_scat_order = 11,
               const Index& t_interp_order = 1) {
  MCGeneral(
      ws, Var::y(ws), Var::mc_iteration_count(ws), Var::mc_error(ws),
      Var::mc_points(ws), Var::mc_source_domain(ws), Var::mc_scat_order(ws),
      Var::mc_antenna(ws), Var::f_grid(ws), Var::f_index(ws),
      Var::sensor_pos(ws), Var::sensor_los(ws), Var::stokes_dim(ws),
      Var::atmosphere_dim(ws), Var::ppath_step_agenda(ws), Var::ppath_lmax(ws),
      Var::ppath_lraytrace(ws), Var::iy_space_agenda(ws),
      Var::surface_rtprop_agenda(ws), Var::propmat_clearsky_agenda(ws),
      Var::p_grid(ws), Var::lat_grid(ws), Var::lon_grid(ws), Var::z_field(ws),
      Var::refellipsoid(ws), Var::z_surface(ws), Var::t_field(ws),
      Var::vmr_field(ws), Var::cloudbox_on(ws), Var::cloudbox_limits(ws),
      Var::pnd_field(ws), Var::scat_data(ws), Var::atmfields_checked(ws),
      Var::atmgeom_checked(ws), Var::scat_data_checked(ws),
      Var::cloudbox_checked(ws), Var::iy_unit(ws), Var::mc_seed(ws),
      Var::mc_std_err(ws), Var::mc_max_time(ws), Var::mc_max_iter(ws),
      Var::mc_min_iter(ws), Var::mc_taustep_limit(ws), l_mc_scat_order,
      t_interp_order, Var::verbosity(ws));
}

/*! A radar 3D foward Monte Carlo radiative algorithm, that allows
for 2D antenna patterns and arbitrary sensor positions.
Surface reflections are currently ignored.

The main output variable *y* and *mc_error* represent the
radar reflectivity integrated over the antenna function, and the
estimated error in this vector, respectively.

Unlike with yActive, the range bins gives the boundaries of
the range bins as either round-trip time or distance from radar.

The WSV *mc_y_tx* gives the polarization state of the
transmitter.

The WSV *mc_max_scatorder* prescribes the maximum scattering
order to consider, after which `photon'-tracing will be
terminated. A value of one calculates only single scattering.

The WSV *mc_max_iter* describes the maximum number of `photons'
used in the simulation (more photons means smaller *mc_error*).
The method will terminate once the max_iter criterium is met.
If negative values are given for these parameters then it is
ignored.

Only "1" and "Ze" are allowed for *iy_unit*. The value of
*mc_error* follows the selection for *iy_unit* (both for in- and
output.

@author Ian S. Adams

@param[in,out] Workspace ws - An ARTS workspace
@param[in] ze_tref - Reference temperature for conversion to Ze. (default:
273.15)
@param[in] k2 - Reference dielectric factor. (default: -1)
@param[in] t_interp_order - Interpolation order of temperature for scattering
data (so far only applied in phase matrix, not in extinction and absorption.
(default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void MCRadar(Workspace& ws, const Numeric& ze_tref = 273.15,
             const Numeric& k2 = -1, const Index& t_interp_order = 1) {
  MCRadar(ws, Var::y(ws), Var::mc_error(ws), Var::mc_antenna(ws),
          Var::f_grid(ws), Var::f_index(ws), Var::sensor_pos(ws),
          Var::sensor_los(ws), Var::stokes_dim(ws), Var::atmosphere_dim(ws),
          Var::ppath_lmax(ws), Var::ppath_step_agenda(ws),
          Var::ppath_lraytrace(ws), Var::propmat_clearsky_agenda(ws),
          Var::p_grid(ws), Var::lat_grid(ws), Var::lon_grid(ws),
          Var::z_field(ws), Var::refellipsoid(ws), Var::z_surface(ws),
          Var::t_field(ws), Var::vmr_field(ws), Var::cloudbox_on(ws),
          Var::cloudbox_limits(ws), Var::pnd_field(ws), Var::scat_data(ws),
          Var::mc_y_tx(ws), Var::range_bins(ws), Var::atmfields_checked(ws),
          Var::atmgeom_checked(ws), Var::scat_data_checked(ws),
          Var::cloudbox_checked(ws), Var::iy_unit(ws),
          Var::mc_max_scatorder(ws), Var::mc_seed(ws), Var::mc_max_iter(ws),
          ze_tref, k2, t_interp_order, Var::verbosity(ws));
}

/*! Sets the value of mc_seed from system time

@author Cory Davis

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void MCSetSeedFromTime(Workspace& ws) {
  MCSetSeedFromTime(Var::mc_seed(ws), Var::verbosity(ws));
}

/*! Interpolation of raw magnetic fields to calculation grids.
Heritage from *AtmFieldsCalc*

Internally, *MagFieldsCalc* applies *GriddedFieldPRegrid* and
*GriddedFieldLatLonRegrid*. Generally, 'half-grid-step' extrapolation
is allowed and applied.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] interp_order - Interpolation order (1=linear interpolation).
(default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void MagFieldsCalc(Workspace& ws, const Index& interp_order = 1) {
  MagFieldsCalc(Var::mag_u_field(ws), Var::mag_v_field(ws),
                Var::mag_w_field(ws), Var::p_grid(ws), Var::lat_grid(ws),
                Var::lon_grid(ws), Var::mag_u_field_raw(ws),
                Var::mag_v_field_raw(ws), Var::mag_w_field_raw(ws),
                Var::atmosphere_dim(ws), interp_order, Var::verbosity(ws));
}

/*! Interpolation of 1D raw atmospheric fields to create 2D or 3D
homogeneous magnetic fields.  Derived from *AtmFieldsCalcExpand1D*

The method works as *MagFieldsCalc*, but accepts only raw 1D
magnetic fields. The raw data is interpolated to *p_grid* and
the obtained values are applied for all latitudes, and also
longitudes for 3D, to create a homogeneous atmosphere.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] interp_order - Interpolation order (1=linear interpolation).
(default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void MagFieldsCalcExpand1D(Workspace& ws, const Index& interp_order = 1) {
  MagFieldsCalcExpand1D(Var::mag_u_field(ws), Var::mag_v_field(ws),
                        Var::mag_w_field(ws), Var::p_grid(ws),
                        Var::lat_grid(ws), Var::lon_grid(ws),
                        Var::mag_u_field_raw(ws), Var::mag_v_field_raw(ws),
                        Var::mag_w_field_raw(ws), Var::atmosphere_dim(ws),
                        interp_order, Var::verbosity(ws));
}

/*! Regrids the rawfield by lat-lon and interpolates to z_field.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] interp_order - Interpolation order (1=linear interpolation).
(default: 1)
@param[in] extrapolating - Extrapolation allowed in interpolation of altitude.
(default: 1e99)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void MagFieldsFromAltitudeRawCalc(Workspace& ws, const Index& interp_order = 1,
                                  const Numeric& extrapolating = 1e99) {
  MagFieldsFromAltitudeRawCalc(
      Var::mag_u_field(ws), Var::mag_v_field(ws), Var::mag_w_field(ws),
      Var::lat_grid(ws), Var::lon_grid(ws), Var::z_field(ws),
      Var::mag_u_field_raw(ws), Var::mag_v_field_raw(ws),
      Var::mag_w_field_raw(ws), interp_order, extrapolating,
      Var::verbosity(ws));
}

/*! Reads magnetic field data from a scenario.

A full set of field components is read (NOTE: fails if scenario
only contains selected field components). The files can be
anywhere, but must all be in the same directory specified by
'basename'. Naming convention for the field component files is
basename.mag_u.xml for the u-component, v- and w-components
accordingly.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] basename - Name of scenario, probably including the full path. For
example: "/data/magnetic_field"

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void MagRawRead(Workspace& ws, const String& basename) {
  MagRawRead(Var::mag_u_field_raw(ws), Var::mag_v_field_raw(ws),
             Var::mag_w_field_raw(ws), basename, Var::verbosity(ws));
}

/*! Forms a matrix containing one column from a vector.

@author Mattias Ekstrom

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to initialize.
@param[in] v - The vector to be copied.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Matrix1ColFromVector(Workspace& ws, Matrix& out, const Vector& v) {
  Matrix1ColFromVector(out, v, Var::verbosity(ws));
}

/*! Forms a matrix containing one row from a vector.

@author Mattias Ekstrom

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to initialize.
@param[in] v - The vector to be copied.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Matrix1RowFromVector(Workspace& ws, Matrix& out, const Vector& v) {
  Matrix1RowFromVector(out, v, Var::verbosity(ws));
}

/*! Forms a matrix containing two columns from two vectors.

The vectors are included as columns in the matrix in the same order
as they are given.

@author Mattias Ekstrom

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to initialize.
@param[in] v1 - The vector to be copied into the first column.
@param[in] v2 - The vector to be copied into the second column.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Matrix2ColFromVectors(Workspace& ws, Matrix& out, const Vector& v1,
                           const Vector& v2) {
  Matrix2ColFromVectors(out, v1, v2, Var::verbosity(ws));
}

/*! Forms a matrix containing two rows from two vectors.

The vectors are included as rows in the matrix in the same order
as they are given.

@author Mattias Ekstrom

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to initialize.
@param[in] v1 - The vector to be copied into the first row.
@param[in] v2 - The vector to be copied into the second row.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Matrix2RowFromVectors(Workspace& ws, Matrix& out, const Vector& v1,
                           const Vector& v2) {
  Matrix2RowFromVectors(out, v1, v2, Var::verbosity(ws));
}

/*! Forms a matrix containing three columns from three vectors.

The vectors are included as columns in the matrix in the same order
as they are given.

@author Mattias Ekstrom

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to initialize.
@param[in] v1 - The vector to be copied into the first column.
@param[in] v2 - The vector to be copied into the second column.
@param[in] v3 - The vector to be copied into the third column.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Matrix3ColFromVectors(Workspace& ws, Matrix& out, const Vector& v1,
                           const Vector& v2, const Vector& v3) {
  Matrix3ColFromVectors(out, v1, v2, v3, Var::verbosity(ws));
}

/*! Forms a matrix containing three rows from three vectors.

The vectors are included as rows in the matrix in the same order
as they are given.

@author Mattias Ekstrom

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to initialize.
@param[in] v1 - The vector to be copied into the first row.
@param[in] v2 - The vector to be copied into the second row.
@param[in] v3 - The vector to be copied into the third row.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Matrix3RowFromVectors(Workspace& ws, Matrix& out, const Vector& v1,
                           const Vector& v2, const Vector& v3) {
  Matrix3RowFromVectors(out, v1, v2, v3, Var::verbosity(ws));
}

/*! Adds a scalar to all elements of a matrix.

The result can either be stored in the same or another matrix.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Output matrix
@param[in] in - Input matrix.
@param[in] value - The value to be added to the matrix.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void MatrixAddScalar(Workspace& ws, Matrix& out, const Matrix& in,
                     const Numeric& value) {
  MatrixAddScalar(out, in, value, Var::verbosity(ws));
}

/*! Sets a matrix to hold cosmic background radiation (CBR).

The CBR is assumed to be un-polarized and Stokes components 2-4
are zero. Number of Stokes components, that equals the number
of columns in the created matrix, is determined by *stokes_dim*.
The number of rows in the created matrix equals the length of the
given frequency vector.

The cosmic radiation is modelled as blackbody radiation for the
temperature given by the global constant COSMIC_BG_TEMP, set in
the file constants.cc. The frequencies are taken from the generic
input vector.

The standard definition, in ARTS, of the Planck function is
followed and the unit of the returned data is W/(m3 * Hz * sr).

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to initialize.
@param[in] f - Frequency vector.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void MatrixCBR(Workspace& ws, Matrix& out, const Vector& f) {
  MatrixCBR(out, Var::stokes_dim(ws), f, Var::verbosity(ws));
}

/*! Creates a matrix by copying a variable of type Sparse.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Created (full) matrix.
@param[in] in - The sparse matrix to be copied.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void MatrixCopySparse(Workspace& ws, Matrix& out, const Sparse& in) {
  MatrixCopySparse(out, in, Var::verbosity(ws));
}

/*! Creates a variable of group Matrix.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void MatrixCreate(Workspace& ws, Matrix& out) {
  MatrixCreate(out, Var::verbosity(ws));
}

/*! Extracts a Matrix from a Tensor3.

Copies page or row or column with given Index from input Tensor3
variable to output Matrix.
Higher order equivalent of *VectorExtractFromMatrix*.

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Extracted matrix.
@param[in] in - Input matrix.
@param[in] i - Index of page or row or column to extract.
@param[in] direction - Direction. "page" or "row" or "column".

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void MatrixExtractFromTensor3(Workspace& ws, Matrix& out, const Tensor3& in,
                              const Index& i, const String& direction) {
  MatrixExtractFromTensor3(out, in, i, direction, Var::verbosity(ws));
}

/*! Turns a covariance matrix into a Matrix.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Dense Matrix.
@param[in] in - Input covariance matrix.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void MatrixFromCovarianceMatrix(Workspace& ws, Matrix& out,
                                const CovarianceMatrix& in) {
  MatrixFromCovarianceMatrix(out, in, Var::verbosity(ws));
}

/*! Returns an identity matrix.

The size if the matrix created is n x n. Default is to return a
true identity matrix (I), but you can also select another value
along the diagonal by setting *value*. That is, the output is
value*I.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Output matrix
@param[in] n - Size of the matrix
@param[in] value - The value along the diagonal. (default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void MatrixIdentity(Workspace& ws, Matrix& out, const Index& n,
                    const Numeric& value = 1) {
  MatrixIdentity(out, n, value, Var::verbosity(ws));
}

/*! Multiply a Matrix with another Matrix and store the result in the result
Matrix.

This just computes the normal Matrix-Matrix product, Y=M*X. It is ok
if Y and X are the same Matrix.

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - The result of the multiplication (dimension mxc).
@param[in] m - The Matrix to multiply (dimension mxn).
@param[in] x - The original Matrix (dimension nxc).

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void MatrixMatrixMultiply(Workspace& ws, Matrix& out, const Matrix& m,
                          const Matrix& x) {
  MatrixMatrixMultiply(out, m, x, Var::verbosity(ws));
}

/*! Sets a matrix to hold blackbody radiation.

The radiation is assumed to be un-polarized and Stokes components
2-4 are zero. Number of Stokes components, that equals the number
of columns in the created matrix, is determined by *stokes_dim*.
The number of rows in the created matrix equals the length of the
given frequency vector.

The standard definition, in ARTS, of the Planck function is
followed and the unit of the returned data is W/(m3 * Hz * sr).

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to initialize.
@param[in] f - Frequency vector.
@param[in] t - Temperature [K].

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void MatrixPlanck(Workspace& ws, Matrix& out, const Vector& f,
                  const Numeric& t) {
  MatrixPlanck(out, Var::stokes_dim(ws), f, t, Var::verbosity(ws));
}

/*! Scales all elements of a matrix with the specified value.

The result can either be stored in the same or another
variable.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Output matrix
@param[in] in - Input matrix.
@param[in] value - The value to be multiplied with the matrix.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void MatrixScale(Workspace& ws, Matrix& out, const Matrix& in,
                 const Numeric& value) {
  MatrixScale(out, in, value, Var::verbosity(ws));
}

/*! Initialize a Matrix from the given list of numbers.

Usage:
   MatrixSet(m1, [1, 2, 3; 4, 5, 6])

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - The newly created matrix
@param[in] value - The values of the newly created matrix. Elements are
separated by commas, rows by semicolons.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void MatrixSet(Workspace& ws, Matrix& out, const Matrix& value) {
  MatrixSet(out, value, Var::verbosity(ws));
}

/*! Creates a matrix and sets all elements to the specified value.

The size is determined by *ncols* and *nrows*.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to initialize.
@param[in] value - Matrix value.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void MatrixSetConstant(Workspace& ws, Matrix& out, const Numeric& value) {
  MatrixSetConstant(out, Var::nrows(ws), Var::ncols(ws), value,
                    Var::verbosity(ws));
}

/*! Sets a matrix to hold unpolarised radiation with unit intensity.

Works as MatrixPlanck where the radiation is set to 1.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to initialize.
@param[in] f - Frequency vector.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void MatrixUnitIntensity(Workspace& ws, Matrix& out, const Vector& f) {
  MatrixUnitIntensity(out, Var::stokes_dim(ws), f, Var::verbosity(ws));
}

/*! Multiply a Matrix with a Vector

Computes the normal Matrix-Vector product, out=m*v. It is ok if out and v
are the same Vector.

@author Stefan Buehler and Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - The result of the multiplication (length m).
@param[in] m - The Matrix to multiply (dimension mxn).
@param[in] v - The original Vector (length n).

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void MatrixVectorMultiply(Workspace& ws, Vector& out, const Matrix& m,
                          const Vector& v) {
  MatrixVectorMultiply(out, m, v, Var::verbosity(ws));
}

/*! Adds a numeric and a value (out = in+value).

The result can either be stored in the same or another numeric.
(in and out can be the same varible, but not out and value)

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Output numeric.
@param[in] in - Input numeric.
@param[in] value - Value to add.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void NumericAdd(Workspace& ws, Numeric& out, const Numeric& in,
                const Numeric& value) {
  NumericAdd(out, in, value, Var::verbosity(ws));
}

/*! Creates a variable of group Numeric.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void NumericCreate(Workspace& ws, Numeric& out) {
  NumericCreate(out, Var::verbosity(ws));
}

/*! Derivs a numeric from a vector, following selected operation.

The following operations can be selected:
  first : Selects the first element of the vector.
   last : Selects the last element of the vector.
    max : Selects the maximum element of the vector.
    min : Selects the minimum element of the vector.
   mean : Calculates the mean of the vector.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Output numeric.
@param[in] in - Input vector.
@param[in] op - Selected operation.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void NumericFromVector(Workspace& ws, Numeric& out, const Vector& in,
                       const String& op) {
  NumericFromVector(out, in, op, Var::verbosity(ws));
}

/*! Inversely scales/divides a numeric with a value (out = in/value).

The result can either be stored in the same or another numeric.
(in and out can be the same varible, but not out and value)

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Output numeric.
@param[in] in - Input numeric.
@param[in] value - Scaling value.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void NumericInvScale(Workspace& ws, Numeric& out, const Numeric& in,
                     const Numeric& value) {
  NumericInvScale(out, in, value, Var::verbosity(ws));
}

/*! Scales/multiplies a numeric with a value (out = in*value).

The result can either be stored in the same or another numeric.
(in and out can be the same varible, but not out and value)

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Output numeric.
@param[in] in - Input numeric.
@param[in] value - Scaling value.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void NumericScale(Workspace& ws, Numeric& out, const Numeric& in,
                  const Numeric& value) {
  NumericScale(out, in, value, Var::verbosity(ws));
}

/*! Sets a numeric workspace variable to the given value.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to initialize.
@param[in] value - The value.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void NumericSet(Workspace& ws, Numeric& out, const Numeric& value) {
  NumericSet(out, value, Var::verbosity(ws));
}

/*! Inversion by the so called optimal estimation method (OEM).

Work in progress ...

The cost function to minimise, including a normalisation with lengthof *y*, is:
   cost = cost_y + cost_x
where
   cost_y = 1/m * [y-yf]' * covmat_se_inv * [y-yf]
   cost_x = 1/m * [x-xa]' * covmat_sx_inv * [x-xa]

 The current implementation provides 3 methods for the minimization of
the cost functional: Linear, Gauss-Newton and Levenberg-Marquardt.
The Gauss-Newton minimizer attempts to find a minimum solution by
fitting a quadratic function to the cost functional. The linear minimizer
is a special case of the Gauss-Newton method, since for a linear forward
model the exact solution of the minimization problem is obtained after
the first step. The Levenberg-Marquardt method adaptively constrains the
search region for the next iteration step by means of the so-called
gamma-factor. This makes the method more suitable for strongly non-linear
problems. If the gamma-factor is 0, Levenberg-Marquardt and Gauss-Newton method
are identical. Each minimization method (li,gn,lm) has an indirect
variant (li_cg,gn_cg,lm_cg), which uses the conjugate gradient solver
for the linear system that has to be solved in each minimzation step.
This of advantage for very large problems, that would otherwise require
the computation of expensive matrix products.

Description of the special input arguments:

*method*
  "li": A linear problem is assumed and a single iteration is performed.
  "li_cg": A linear problem is assumed and solved using the CG solver.
  "gn": Non-linear, with Gauss-Newton iteration scheme.
  "gn_cg": Non-linear, with Gauss-Newton and conjugate gradient solver.
  "lm": Non-linear, with Levenberg-Marquardt (LM) iteration scheme.
  "lm_cg": Non-linear, with Levenberg-Marquardt (LM) iteration scheme and
conjugate gradient solver. *max_start_cost* No inversion is done if the cost
matching the a priori state is above this value. If set to a negative value, all
values are accepted. This argument also controls if the start cost is
calculated. If set to <= 0, the start cost in *oem_diagnostics* is set to NaN
  when using "li" and "gn".
*x_norm*
  A normalisation vector for *x*. A normalisation of *x* can be needed
  due to limited numerical precision. If this vector is set to be empty
  no normalisation is done (defualt case). Otherwise, this must be a
  vector with same length as *x*, just having values above zero.
  Elementwise division between *x* and *x_norm* (x./x_norm) shall give
  a vector where all values are in the order of unity. Maybe the best
  way to set *x_norm* is x_norm = sqrt( diag( Sx ) ).
*max_iter*
  Maximum number of iterations to perform. No effect for "li".
*stop_dx*
  Iteration stop criterion. The criterion used is the same as given
  in Rodgers' "Inverse Methods for Atmospheric Sounding"
*lm_ga_settings*
  Settings controlling the gamma factor, part of the "LM" method.
  This is a vector of length 6, having the elements (0-based index):
    0: Start value.
    1: Fractional decrease after succesfull iteration.
    2: Fractional increase after unsuccessful iteration.
    3: Maximum allowed value. If the value is passed, the inversion
       is halted.
    4: Lower treshold. If the threshold is passed, gamma is set to zero.
       If gamma must be increased from zero, gamma is set to this value.
    5: Gamma limit. This is an additional stop criterion. Convergence
       is not considered until there has been one succesful iteration
       having a gamma <= this value.
  The default setting triggers an error if "lm" is selected.
*clear matrices*
   With this flag set to 1, *jacobian* and *dxdy* are returned as empty
   matrices.
*display_progress*
   Controls if there is any screen output. The overall report level
   is ignored by this WSM.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] method - Iteration method. For this and all options below, see
further above.
@param[in] max_start_cost - Maximum allowed value of cost function at start.
(default: std::numeric_limits<Numeric>::infinity())
@param[in] x_norm - Normalisation of Sx. (default: {})
@param[in] max_iter - Maximum number of iterations. (default: 10)
@param[in] stop_dx - Stop criterion for iterative inversions. (default: 0.01)
@param[in] lm_ga_settings - Settings associated with the ga factor of the LM
method. (default: {})
@param[in] clear_matrices - An option to save memory. (default: 0)
@param[in] display_progress - Flag to control if inversion diagnostics shall be
printed on the screen. (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void OEM(
    Workspace& ws, const String& method,
    const Numeric& max_start_cost = std::numeric_limits<Numeric>::infinity(),
    const Vector& x_norm = {}, const Index& max_iter = 10,
    const Numeric& stop_dx = 0.01, const Vector& lm_ga_settings = {},
    const Index& clear_matrices = 0, const Index& display_progress = 0) {
  OEM(ws, Var::x(ws), Var::yf(ws), Var::jacobian(ws), Var::dxdy(ws),
      Var::oem_diagnostics(ws), Var::lm_ga_history(ws), Var::oem_errors(ws),
      Var::xa(ws), Var::covmat_sx(ws), Var::y(ws), Var::covmat_se(ws),
      Var::jacobian_quantities(ws), Var::inversion_iterate_agenda(ws), method,
      max_start_cost, x_norm, max_iter, stop_dx, lm_ga_settings, clear_matrices,
      display_progress, Var::verbosity(ws));
}

/*! Optimization of the pressure grid for RT calculation.
The methods consists of three parts:
1) Calculate the single scattering albedo and the scattering opticalthickness
from the scattering and absorption species. 2) Enhance z_field according to the
two thresholds sgl_alb_max and tau_scat_max.If the resulting cloudbox size is
bigger than cloudbox_size_max, this step is repeated with a higher threshold of
tau_scat_max. 3) Interpolate all variables used in doit_mono_agenda to the new
z_field This method should be called inside *doit_mono_agenda*, right before
*cloudbox_field_monoIterate*. It can only be used if *ScatSpeciesMerge* has been
called and if it is called, *cloudbox_field_monoOptimizeReverse* has to be
called right after *cloudbox_field_monoIterate* to interpolate
*cloudbox_field_mono* back to the original size.
Optimization currently only works with *stokes_dim* = 1 .

@author Jakob Doerr

@param[in,out] Workspace ws - An ARTS workspace
@param[in] tau_scat_max - Maximum scattering optical thickness (default: 0.1)
@param[in] sgl_alb_max - Maximum single scattering albedo (default: 0.9)
@param[in] cloudbox_size_max - Maximum cloudbox size (default: 200)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void OptimizeDoitPressureGrid(Workspace& ws, const Numeric& tau_scat_max = 0.1,
                              const Numeric& sgl_alb_max = 0.9,
                              const Index& cloudbox_size_max = 200) {
  OptimizeDoitPressureGrid(
      ws, Var::p_grid(ws), Var::pnd_field(ws), Var::t_field(ws),
      Var::scat_data_mono(ws), Var::z_field(ws), Var::cloudbox_limits(ws),
      Var::cloudbox_field_mono(ws), Var::pha_mat_doit(ws), Var::vmr_field(ws),
      Var::p_grid_orig(ws), Var::f_grid(ws), Var::f_index(ws),
      Var::propmat_clearsky_agenda(ws), tau_scat_max, sgl_alb_max,
      cloudbox_size_max, Var::verbosity(ws));
}

/*! Simple conversion from altitude to pressure.

This function converts a vector of altitudes to an approximate vector
of corresponding pressures. The formula used to convert altitide z to height
 is:
p = 10.0^(5.0 - z / 1600)

Note that all altitude values in the vector must be less than 120 km,
 otherwise an error will be thrown.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[out] p_grid - Approximate pressures of corresponding to given altitudes.
@param[in] z_grid - Altitude grid.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void PFromZSimple(Workspace& ws, Vector& p_grid, const Vector& z_grid) {
  PFromZSimple(p_grid, z_grid, Var::verbosity(ws));
}

/*! Creates a variable of group Ppath.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void PpathCreate(Workspace& ws, Ppath& out) {
  PpathCreate(out, Var::verbosity(ws));
}

/*! Prints a variable on the screen.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] in - Variable to be printed.
@param[in] level - Output level to use. (default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void Print(Workspace& ws, const Any_0& in, const Index& level = 1) {
  Print(in, level, Var::verbosity(ws));
}

/*! Prints (most) physical constants used in ARTS.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void PrintPhysicalConstants(Workspace& ws) {
  PrintPhysicalConstants(Var::verbosity(ws));
}

/*! Prints a list of the workspace variables.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] only_allocated - Flag for printing either all variables (0) or only
allocated ones (1). (default: 1)
@param[in] level - Output level to use. (default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void PrintWorkspace(Workspace& ws, const Index& only_allocated = 1,
                    const Index& level = 1) {
  PrintWorkspace(ws, only_allocated, level, Var::verbosity(ws));
}

/*! Creates a variable of group PropagationMatrix.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void PropagationMatrixCreate(Workspace& ws, PropagationMatrix& out) {
  PropagationMatrixCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group QuantumIdentifier.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void QuantumIdentifierCreate(Workspace& ws, QuantumIdentifier& out) {
  QuantumIdentifierCreate(out, Var::verbosity(ws));
}

/*! Sets a QuantumIdentifier workspace variable to the given value
by converting the input String

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to initialize.
@param[in] string_initializer - The string representing the value.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void QuantumIdentifierSet(Workspace& ws, QuantumIdentifier& out,
                          const String& string_initializer) {
  QuantumIdentifierSet(out, string_initializer, Var::verbosity(ws));
}

/*! Interface to the PolRadTran RT4 scattering solver (by F. Evans).

RT4 provides the radiation field (*cloudbox_field*) from a vector
1D scattering solution assuming a plane-parallel atmosphere (flat
Earth). It calculates up to two Stokes parameters (*stokes_dim*<=2),
i.e., all azimuthally randomly oriented particles are allowed (this
also includes macroscopically isotropic particles). Refraction is
not taken into account.

The scattering solution is internally obtained over the full
(plane-parallel) atmosphere, i.e. not confined to the cloudbox.
However, the radiation field output is limited to the cloudbox.
This allows to consider clearsky RT through a non-spherical
atmosphere outside the cloudbox improving the RT solution for
non-plane-parallel media compared to the plain RT4 output.

*nstreams* is the number of polar angles taken into account
internally in the scattering solution. That is, *nstreams*
determines the angular resolution, hence the accuracy, of the
scattering solution. The more anisotropic the bulk scattering
matrix, the more streams are required. The computational burden
increases approximately with the third power of *nstreams*.
The default value (*nstreams*=16) was found to be sufficient for
most microwave scattering calculations. It is likely insufficient
for IR calculations involving ice clouds, though.

Here, *za_grid* is NOT an input parameter, but output, and its
size equals *nstreams* or *nstreams*+2 (Gauss-Legendre and Double
Gauss quadratures in case *add_straight_angles*=1) (the reason is
that the computational burden is high for additional angles,
regardless whether they are quadrature angles or not; hence the
quadrature angles supplemented with 0 and 180deg are considered to
provide the best radiation field for a given effort).

The *auto_inc_nstreams* feature can be used to increase the number
of streams used internally in the scattering solution when found
necessary.
NOTE: this number-of-streams increase is only internally - the
angular dimension of the output *cloudbox_field* is fixed to the
*nstreams* given as input to this WSM.

Quadrature methods available are: 'L'obatto, 'G'auss-Legendre and
'D'ouble Gauss quadrature.

This WSM applies *surface_rtprop_agenda* to derive reflection
matrix and surface emission vector that are directly feed into
RT4's core solver (instead of their RT4-internal calculation as
used by *RT4CalcWithRT4Surface*).

Known issues of ARTS implementation:
- TOA incoming radiation is so far assumed as blackbody cosmic
  background (temperature taken from the ARTS-internal constant).

The keyword *pfct_method* allows to choose the method to extract the
scattering matrix. 'interpolate' considers temperature dependence,
others neglect it by chosing one specific temperature grid point
from the single scattering data: 'low' choses the lowest T-point,
'high' the highest T-point, and 'median' the median T-point. As
different scattering elements can have different temperature grids,
the actual temperature value used can differ between the scattering
elements.
Note that this keyword solely affects the scattering matrix;
extinction matrix and absorption vector are always interpolated to
the actual temperature.

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] nstreams - Number of polar angle directions (streams) in RT4 solution
(must be an even number). (default: 16)
@param[in] pfct_method - Flag which method to apply to derive phase function
(for available options see above). (default: "median")
@param[in] quad_type - Flag which quadrature to apply in RT4 solution (for
available options see above). (default: "D")
@param[in] add_straight_angles - Flag whether to include nadir and zenith as
explicit directions (only effective for quad_type G and D). (default: 1)
@param[in] pfct_aa_grid_size - Number of azimuthal angle grid points to consider
in Fourier series decomposition of scattering matrix (only applied for randomly
oriented scattering elements) (default: 19)
@param[in] auto_inc_nstreams - Flag whether to internally increase nstreams
(individually per frequency) if norm of (bulk) scattering matrix is not
preserved properly. If 0, no adaptation is done. Else *auto_inc_nstreams* gives
the maximum number of streams to increase to. Note that the output
*cloudbox_field* remains with angular dimension of *nstreams*, only the internal
solution is adapted (and then interpolated to the lower-resolution output
angular grid). (default: 0)
@param[in] robust - For *auto_inc_nstreams*>0, flag whether to not fail even if
scattering matrix norm is not preserved when maximum stream number is reached.
Internal RT4 calculations is then performed with nstreams=*auto_inc_nstreams*.
(default: 0)
@param[in] za_interp_order - For *auto_inc_nstreams*>0, polar angle
interpolation order for interpolation from internal increased stream to
originally requested nstreams-ifield. (default: 1)
@param[in] cos_za_interp - For *auto_inc_nstreams*>0, flag whether to do polar
angle interpolation in cosine (='mu') space. (default: 0)
@param[in] max_delta_tau - Maximum optical depth of infinitesimal layer (where
single scattering approximation is assumed to apply). (default: 1e-6)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void RT4Calc(Workspace& ws, const Index& nstreams = 16,
             const String& pfct_method = "median",
             const String& quad_type = "D",
             const Index& add_straight_angles = 1,
             const Index& pfct_aa_grid_size = 19,
             const Index& auto_inc_nstreams = 0, const Index& robust = 0,
             const Index& za_interp_order = 1, const Index& cos_za_interp = 0,
             const Numeric& max_delta_tau = 1e-6) {
  RT4Calc(ws, Var::cloudbox_field(ws), Var::za_grid(ws), Var::aa_grid(ws),
          Var::atmfields_checked(ws), Var::atmgeom_checked(ws),
          Var::scat_data_checked(ws), Var::cloudbox_checked(ws),
          Var::cloudbox_on(ws), Var::cloudbox_limits(ws),
          Var::propmat_clearsky_agenda(ws), Var::surface_rtprop_agenda(ws),
          Var::atmosphere_dim(ws), Var::pnd_field(ws), Var::t_field(ws),
          Var::z_field(ws), Var::vmr_field(ws), Var::p_grid(ws),
          Var::scat_data(ws), Var::f_grid(ws), Var::stokes_dim(ws),
          Var::z_surface(ws), nstreams, pfct_method, quad_type,
          add_straight_angles, pfct_aa_grid_size, auto_inc_nstreams, robust,
          za_interp_order, cos_za_interp, max_delta_tau, Var::verbosity(ws));
}

/*! As RT4Calc except for using RT4's proprietary surface type handling.

This WSM is only indented for testing purposes.

The following surface type/property methods are available and
require the the following input:
- 'L'ambertian: *surface_scalar_reflectivity*, *surface_skin_t*
- 'F'resnel: *surface_complex_refr_index*, *surface_skin_t*
- 'S'pecular: *surface_reflectivity*, *surface_skin_t*
'L' and 'F' use proprietary RT4 methods, 'S' uses RT4's Fresnel
methods modified to behave similar to ARTS'
*surfaceFlatReflectivity*.

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] nstreams - Number of polar angle directions (streams) in RT4 solution
(must be an even number). (default: 16)
@param[in] pfct_method - Flag which method to apply to derive phase function
(for available options see above). (default: "median")
@param[in] ground_type - Flag which surface type/surface property method to use
(for available options see above). (default: "A")
@param[in] quad_type - Flag which quadrature to apply in RT4 solution (for
available options see above). (default: "D")
@param[in] add_straight_angles - Flag whether to include nadir and zenith as
explicit directions (only effective for quad_type G and D). (default: 1)
@param[in] pfct_aa_grid_size - Number of azimuthal angle grid points to consider
in Fourier series decomposition of scattering matrix (only applied for randomly
oriented scattering elements) (default: 19)
@param[in] auto_inc_nstreams - Flag whether to internally increase nstreams
(individually per frequency) if norm of (bulk) scattering matrix is not
preserved properly. If 0, no adaptation is done. Else *auto_inc_nstreams* gives
the maximum number of streams to increase to. (default: 0)
@param[in] robust - For *auto_inc_nstreams*>0, flag whether to not fail even if
scattering matrix norm is not preserved when maximum stream number is reached.
Internal RT4 calculations is then performed with nstreams=*auto_inc_nstreams*.
(default: 0)
@param[in] za_interp_order - For *auto_inc_nstreams*>0, polar angle
interpolation order for interpolation from internal increased stream to
originally requested nstreams-ifield. (default: 1)
@param[in] cos_za_interp - For *auto_inc_nstreams*>0, flag whether to do polar
angle interpolation in cosine (='mu') space. (default: 0)
@param[in] max_delta_tau - Maximum optical depth of infinitesimal layer (where
single scattering approximation is assumed to apply). (default: 1e-6)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void RT4CalcWithRT4Surface(
    Workspace& ws, const Index& nstreams = 16,
    const String& pfct_method = "median", const String& ground_type = "A",
    const String& quad_type = "D", const Index& add_straight_angles = 1,
    const Index& pfct_aa_grid_size = 19, const Index& auto_inc_nstreams = 0,
    const Index& robust = 0, const Index& za_interp_order = 1,
    const Index& cos_za_interp = 0, const Numeric& max_delta_tau = 1e-6) {
  RT4CalcWithRT4Surface(
      ws, Var::cloudbox_field(ws), Var::za_grid(ws), Var::aa_grid(ws),
      Var::atmfields_checked(ws), Var::atmgeom_checked(ws),
      Var::scat_data_checked(ws), Var::cloudbox_checked(ws),
      Var::cloudbox_on(ws), Var::cloudbox_limits(ws),
      Var::propmat_clearsky_agenda(ws), Var::atmosphere_dim(ws),
      Var::pnd_field(ws), Var::t_field(ws), Var::z_field(ws),
      Var::vmr_field(ws), Var::p_grid(ws), Var::scat_data(ws), Var::f_grid(ws),
      Var::stokes_dim(ws), Var::z_surface(ws), Var::surface_skin_t(ws),
      Var::surface_scalar_reflectivity(ws), Var::surface_reflectivity(ws),
      Var::surface_complex_refr_index(ws), nstreams, pfct_method, ground_type,
      quad_type, add_straight_angles, pfct_aa_grid_size, auto_inc_nstreams,
      robust, za_interp_order, cos_za_interp, max_delta_tau,
      Var::verbosity(ws));
}

/*! RT4 validation test.

Executes test case testc shipped with PolRadTran/RT4 code (but uses
data files converted to arts-xml). Output written to (xml-)file.

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out_rad - RT4 testc calculation results.
@param[in] datapath - Folder containing arts-xml converted test case input data.
(default: "artscomponents/polradtran/testdata/")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void RT4Test(Workspace& ws, Tensor4& out_rad,
             const String& datapath = "artscomponents/polradtran/testdata/") {
  RT4Test(out_rad, datapath, Var::verbosity(ws));
}

/*! Integrates fields like *spectral_irradiance_field* or *cloudbox_field*
over frequency.
Important, the first dimension must be the frequency dimension!
If a field  like *cloudbox_field* is input, the stokes dimension
is also removed.

@author Manfred Brath

@param[in,out] Workspace ws - An ARTS workspace
@param[out] radiation_field - TBD
@param[in] spectral_radiation_field - TBD

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0, typename Any_1>
void RadiationFieldSpectralIntegrate(Workspace& ws, Any_0& radiation_field,
                                     const Any_1& spectral_radiation_field) {
  RadiationFieldSpectralIntegrate(radiation_field, Var::f_grid(ws),
                                  spectral_radiation_field, Var::verbosity(ws));
}

/*! Creates a variable of group RadiationVector.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void RadiationVectorCreate(Workspace& ws, RadiationVector& out) {
  RadiationVectorCreate(out, Var::verbosity(ws));
}

/*! Adds a Rational and a value (out = in+value).

The result can either be stored in the same or another Rational.
(in and out can be the same varible, but not out and value)

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Output Rational.
@param[in] in - Input Rational.
@param[in] value - Value to add.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void RationalAdd(Workspace& ws, Rational& out, const Rational& in,
                 const Rational& value) {
  RationalAdd(out, in, value, Var::verbosity(ws));
}

/*! Creates a variable of group Rational.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void RationalCreate(Workspace& ws, Rational& out) {
  RationalCreate(out, Var::verbosity(ws));
}

/*! Inversely scales/divides a Rational with a value (out = in/value).

The result can either be stored in the same or another Rational.
(in and out can be the same varible, but not out and value)

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Output Rational.
@param[in] in - Input Rational.
@param[in] value - Scaling Rational.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void RationalInvScale(Workspace& ws, Rational& out, const Rational& in,
                      const Rational& value) {
  RationalInvScale(out, in, value, Var::verbosity(ws));
}

/*! Scales/multiplies a Rational with a value (out = in*value).

The result can either be stored in the same or another Rational.
(in and out can be the same varible, but not out and value)

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Output Rational.
@param[in] in - Input Rational.
@param[in] value - Scaling value.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void RationalScale(Workspace& ws, Rational& out, const Rational& in,
                   const Rational& value) {
  RationalScale(out, in, value, Var::verbosity(ws));
}

/*! Sets a Rational workspace variable to the given value.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to initialize.
@param[in] numerator - The numerator.
@param[in] denominator - The denominator. (default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void RationalSet(Workspace& ws, Rational& out, const Index& numerator,
                 const Index& denominator = 1) {
  RationalSet(out, numerator, denominator, Var::verbosity(ws));
}

/*! Reads an old ArrayOfLineRecord ARTSCAT file

Note that the ARTSCAT-5 had quantum numbers and options
stored inside it but that the options will overwrite that
information.  Be careful setting the options!

@author Stefan Buehler
@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] filename - Name of the ARTSCAT file
@param[in] fmin - Minimum frequency of read lines (default: 0)
@param[in] fmax - Maximum frequency of read lines (default: 1e99)
@param[in] globalquantumnumbers - Global quantum number list (space-separated)
(default: "")
@param[in] localquantumnumbers - Local quantum number list (space-separated)
(default: "")
@param[in] normalization_option - Normalization option, see
*abs_linesSetNormalization* (default: "None")
@param[in] mirroring_option - Mirroring option, see *abs_linesSetMirroring*
(default: "None")
@param[in] population_option - Population option, see *abs_linesSetPopulation*
(default: "LTE")
@param[in] lineshapetype_option - Lineshape option, see
*abs_linesSetLineShapeType* (default: "VP")
@param[in] cutoff_option - Cutoff option, see *abs_linesSetCutoff* (default:
"None")
@param[in] cutoff_value - Cutoff value, see *abs_linesSetCutoff* (default:
750e9)
@param[in] linemixinglimit_value - Line mixing limit, see
*abs_linesSetLinemixingLimit* (default: -1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ReadARTSCAT(Workspace& ws, const String& filename, const Numeric& fmin = 0,
                 const Numeric& fmax = 1e99,
                 const String& globalquantumnumbers = "",
                 const String& localquantumnumbers = "",
                 const String& normalization_option = "None",
                 const String& mirroring_option = "None",
                 const String& population_option = "LTE",
                 const String& lineshapetype_option = "VP",
                 const String& cutoff_option = "None",
                 const Numeric& cutoff_value = 750e9,
                 const Numeric& linemixinglimit_value = -1) {
  ReadARTSCAT(Var::abs_lines(ws), filename, fmin, fmax, globalquantumnumbers,
              localquantumnumbers, normalization_option, mirroring_option,
              population_option, lineshapetype_option, cutoff_option,
              cutoff_value, linemixinglimit_value, Var::verbosity(ws));
}

/*! Reads an old Array<ArrayOfLineRecord> ARTSCAT file.

Note that the ARTSCAT-5 had quantum numbers and options
stored inside it but that the options will overwrite that
information.  Be careful setting the options!

@author Stefan Buehler
@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] filename - Name of the ARTSCAT file
@param[in] fmin - Minimum frequency of read lines (default: 0)
@param[in] fmax - Maximum frequency of read lines (default: 1e99)
@param[in] globalquantumnumbers - Global quantum number list (space-separated)
(default: "")
@param[in] localquantumnumbers - Local quantum number list (space-separated)
(default: "")
@param[in] normalization_option - Normalization option, see
*abs_linesSetNormalization* (default: "None")
@param[in] mirroring_option - Mirroring option, see *abs_linesSetMirroring*
(default: "None")
@param[in] population_option - Population option, see *abs_linesSetPopulation*
(default: "LTE")
@param[in] lineshapetype_option - Lineshape option, see
*abs_linesSetLineShapeType* (default: "VP")
@param[in] cutoff_option - Cutoff option, see *abs_linesSetCutoff* (default:
"None")
@param[in] cutoff_value - Cutoff value, see *abs_linesSetCutoff* (default:
750e9)
@param[in] linemixinglimit_value - Line mixing limit, see
*abs_linesSetLinemixingLimit* (default: -1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ReadArrayOfARTSCAT(Workspace& ws, const String& filename,
                        const Numeric& fmin = 0, const Numeric& fmax = 1e99,
                        const String& globalquantumnumbers = "",
                        const String& localquantumnumbers = "",
                        const String& normalization_option = "None",
                        const String& mirroring_option = "None",
                        const String& population_option = "LTE",
                        const String& lineshapetype_option = "VP",
                        const String& cutoff_option = "None",
                        const Numeric& cutoff_value = 750e9,
                        const Numeric& linemixinglimit_value = -1) {
  ReadArrayOfARTSCAT(Var::abs_lines(ws), filename, fmin, fmax,
                     globalquantumnumbers, localquantumnumbers,
                     normalization_option, mirroring_option, population_option,
                     lineshapetype_option, cutoff_option, cutoff_value,
                     linemixinglimit_value, Var::verbosity(ws));
}

/*! Reads a HITRAN .par file.

The HITRAN type switch can be:
        "Pre2004"	-	for old format
        "Post2004"	-	for new format
        "Online"	-	for the online format with quantum numbers
(highly experimental)

Be careful setting the options!

@author Hermann Berg
@author Thomas Kuhn
@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] filename - Name of the HITRAN file
@param[in] fmin - Minimum frequency of read lines (default: 0)
@param[in] fmax - Maximum frequency of read lines (default: 1e99)
@param[in] globalquantumnumbers - Global quantum number list (space-separated)
(default: "")
@param[in] localquantumnumbers - Local quantum number list (space-separated)
(default: "")
@param[in] hitran_type - Method to use to read the line data (default:
"Post2004")
@param[in] normalization_option - Normalization option, see
*abs_linesSetNormalization* (default: "None")
@param[in] mirroring_option - Mirroring option, see *abs_linesSetMirroring*
(default: "None")
@param[in] population_option - Population option, see *abs_linesSetPopulation*
(default: "LTE")
@param[in] lineshapetype_option - Lineshape option, see
*abs_linesSetLineShapeType* (default: "VP")
@param[in] cutoff_option - Cutoff option, see *abs_linesSetCutoff* (default:
"None")
@param[in] cutoff_value - Cutoff value, see *abs_linesSetCutoff* (default:
750e9)
@param[in] linemixinglimit_value - Line mixing limit, see
*abs_linesSetLinemixingLimit* (default: -1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ReadHITRAN(Workspace& ws, const String& filename, const Numeric& fmin = 0,
                const Numeric& fmax = 1e99,
                const String& globalquantumnumbers = "",
                const String& localquantumnumbers = "",
                const String& hitran_type = "Post2004",
                const String& normalization_option = "None",
                const String& mirroring_option = "None",
                const String& population_option = "LTE",
                const String& lineshapetype_option = "VP",
                const String& cutoff_option = "None",
                const Numeric& cutoff_value = 750e9,
                const Numeric& linemixinglimit_value = -1) {
  ReadHITRAN(Var::abs_lines(ws), filename, fmin, fmax, globalquantumnumbers,
             localquantumnumbers, hitran_type, normalization_option,
             mirroring_option, population_option, lineshapetype_option,
             cutoff_option, cutoff_value, linemixinglimit_value,
             Var::verbosity(ws));
}

/*! Reads a JPL file.

Be careful setting the options!

@author Thomas Kuhn
@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] filename - Name of the JPL file
@param[in] fmin - Minimum frequency of read lines (default: 0)
@param[in] fmax - Maximum frequency of read lines (default: 1e99)
@param[in] globalquantumnumbers - Global quantum number list (space-separated)
(default: "")
@param[in] localquantumnumbers - Local quantum number list (space-separated)
(default: "")
@param[in] normalization_option - Normalization option, see
*abs_linesSetNormalization* (default: "None")
@param[in] mirroring_option - Mirroring option, see *abs_linesSetMirroring*
(default: "None")
@param[in] population_option - Population option, see *abs_linesSetPopulation*
(default: "LTE")
@param[in] lineshapetype_option - Lineshape option, see
*abs_linesSetLineShapeType* (default: "VP")
@param[in] cutoff_option - Cutoff option, see *abs_linesSetCutoff* (default:
"None")
@param[in] cutoff_value - Cutoff value, see *abs_linesSetCutoff* (default:
750e9)
@param[in] linemixinglimit_value - Line mixing limit, see
*abs_linesSetLinemixingLimit* (default: -1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ReadJPL(Workspace& ws, const String& filename, const Numeric& fmin = 0,
             const Numeric& fmax = 1e99,
             const String& globalquantumnumbers = "",
             const String& localquantumnumbers = "",
             const String& normalization_option = "None",
             const String& mirroring_option = "None",
             const String& population_option = "LTE",
             const String& lineshapetype_option = "VP",
             const String& cutoff_option = "None",
             const Numeric& cutoff_value = 750e9,
             const Numeric& linemixinglimit_value = -1) {
  ReadJPL(Var::abs_lines(ws), filename, fmin, fmax, globalquantumnumbers,
          localquantumnumbers, normalization_option, mirroring_option,
          population_option, lineshapetype_option, cutoff_option, cutoff_value,
          linemixinglimit_value, Var::verbosity(ws));
}

/*! Reads a LBLRTM file.

Be careful setting the options!

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] filename - Name of the LBLRTM file
@param[in] fmin - Minimum frequency of read lines (default: 0)
@param[in] fmax - Maximum frequency of read lines (default: 1e99)
@param[in] globalquantumnumbers - Global quantum number list (space-separated)
(default: "")
@param[in] localquantumnumbers - Local quantum number list (space-separated)
(default: "")
@param[in] normalization_option - Normalization option, see
*abs_linesSetNormalization* (default: "None")
@param[in] mirroring_option - Mirroring option, see *abs_linesSetMirroring*
(default: "None")
@param[in] population_option - Population option, see *abs_linesSetPopulation*
(default: "LTE")
@param[in] lineshapetype_option - Lineshape option, see
*abs_linesSetLineShapeType* (default: "VP")
@param[in] cutoff_option - Cutoff option, see *abs_linesSetCutoff* (default:
"None")
@param[in] cutoff_value - Cutoff value, see *abs_linesSetCutoff* (default:
750e9)
@param[in] linemixinglimit_value - Line mixing limit, see
*abs_linesSetLinemixingLimit* (default: -1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ReadLBLRTM(Workspace& ws, const String& filename, const Numeric& fmin = 0,
                const Numeric& fmax = 1e99,
                const String& globalquantumnumbers = "",
                const String& localquantumnumbers = "",
                const String& normalization_option = "None",
                const String& mirroring_option = "None",
                const String& population_option = "LTE",
                const String& lineshapetype_option = "VP",
                const String& cutoff_option = "None",
                const Numeric& cutoff_value = 750e9,
                const Numeric& linemixinglimit_value = -1) {
  ReadLBLRTM(Var::abs_lines(ws), filename, fmin, fmax, globalquantumnumbers,
             localquantumnumbers, normalization_option, mirroring_option,
             population_option, lineshapetype_option, cutoff_option,
             cutoff_value, linemixinglimit_value, Var::verbosity(ws));
}

/*! Reads a Mytran2 file.

Be careful setting the options!

@author Axel von Engeln
@author Stefan Buehler
@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] filename - Name of the Mytran2 file
@param[in] fmin - Minimum frequency of read lines (default: 0)
@param[in] fmax - Maximum frequency of read lines (default: 1e99)
@param[in] globalquantumnumbers - Global quantum number list (space-separated)
(default: "")
@param[in] localquantumnumbers - Local quantum number list (space-separated)
(default: "")
@param[in] normalization_option - Normalization option, see
*abs_linesSetNormalization* (default: "None")
@param[in] mirroring_option - Mirroring option, see *abs_linesSetMirroring*
(default: "None")
@param[in] population_option - Population option, see *abs_linesSetPopulation*
(default: "LTE")
@param[in] lineshapetype_option - Lineshape option, see
*abs_linesSetLineShapeType* (default: "VP")
@param[in] cutoff_option - Cutoff option, see *abs_linesSetCutoff* (default:
"None")
@param[in] cutoff_value - Cutoff value, see *abs_linesSetCutoff* (default:
750e9)
@param[in] linemixinglimit_value - Line mixing limit, see
*abs_linesSetLinemixingLimit* (default: -1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ReadMytran2(Workspace& ws, const String& filename, const Numeric& fmin = 0,
                 const Numeric& fmax = 1e99,
                 const String& globalquantumnumbers = "",
                 const String& localquantumnumbers = "",
                 const String& normalization_option = "None",
                 const String& mirroring_option = "None",
                 const String& population_option = "LTE",
                 const String& lineshapetype_option = "VP",
                 const String& cutoff_option = "None",
                 const Numeric& cutoff_value = 750e9,
                 const Numeric& linemixinglimit_value = -1) {
  ReadMytran2(Var::abs_lines(ws), filename, fmin, fmax, globalquantumnumbers,
              localquantumnumbers, normalization_option, mirroring_option,
              population_option, lineshapetype_option, cutoff_option,
              cutoff_value, linemixinglimit_value, Var::verbosity(ws));
}

/*! Reads a workspace variable from a NetCDF file.

This method can read variables of any group.

If the filename is omitted, the variable is read
from <basename>.<variable_name>.nc.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to be read.
@param[in] filename - Name of the NetCDF file.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void ReadNetCDF(Workspace& ws, std::pair<Any_0, String>& out,
                const std::pair<String, String>& filename) {
  ReadNetCDF(out.first, out.second, filename.first, filename.second,
             Var::verbosity(ws));
}

/*! Reads several old ArrayOfLineRecord ARTSCAT file

Note that the ARTSCAT-5 had quantum numbers and options
stored inside it but that the options will overwrite that
information.  Be careful setting the options!

@author Oliver Lemke
@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] basename - Path to the files
@param[in] fmin - Minimum frequency of read lines (default: 0)
@param[in] fmax - Maximum frequency of read lines (default: 1e99)
@param[in] globalquantumnumbers - Global quantum number list (space-separated)
(default: "")
@param[in] localquantumnumbers - Local quantum number list (space-separated)
(default: "")
@param[in] ignore_missing - Ignores instead of throws if an *abs_species* is
missing (default: 0)
@param[in] normalization_option - Normalization option, see
*abs_linesSetNormalization* (default: "None")
@param[in] mirroring_option - Mirroring option, see *abs_linesSetMirroring*
(default: "None")
@param[in] population_option - Population option, see *abs_linesSetPopulation*
(default: "LTE")
@param[in] lineshapetype_option - Lineshape option, see
*abs_linesSetLineShapeType* (default: "VP")
@param[in] cutoff_option - Cutoff option, see *abs_linesSetCutoff* (default:
"None")
@param[in] cutoff_value - Cutoff value, see *abs_linesSetCutoff* (default:
750e9)
@param[in] linemixinglimit_value - Line mixing limit, see
*abs_linesSetLinemixingLimit* (default: -1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ReadSplitARTSCAT(Workspace& ws, const String& basename,
                      const Numeric& fmin = 0, const Numeric& fmax = 1e99,
                      const String& globalquantumnumbers = "",
                      const String& localquantumnumbers = "",
                      const Index& ignore_missing = 0,
                      const String& normalization_option = "None",
                      const String& mirroring_option = "None",
                      const String& population_option = "LTE",
                      const String& lineshapetype_option = "VP",
                      const String& cutoff_option = "None",
                      const Numeric& cutoff_value = 750e9,
                      const Numeric& linemixinglimit_value = -1) {
  ReadSplitARTSCAT(Var::abs_lines(ws), Var::abs_species(ws), basename, fmin,
                   fmax, globalquantumnumbers, localquantumnumbers,
                   ignore_missing, normalization_option, mirroring_option,
                   population_option, lineshapetype_option, cutoff_option,
                   cutoff_value, linemixinglimit_value, Var::verbosity(ws));
}

/*! Reads a workspace variable from an XML file.

This method can read variables of any group.

If the filename is omitted, the variable is read
from <basename>.<variable_name>.xml.
If the given filename does not exist, this method will
also look for files with an added .xml, .xml.gz and .gz extension

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to be read.
@param[in] filename - Name of the XML file. (default: "")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void ReadXML(Workspace& ws, std::pair<Any_0, String>& out,
             const std::pair<String, String>& filename = {"", "filename"}) {
  ReadXML(out.first, out.second, filename.first, filename.second,
          Var::verbosity(ws));
}

/*! As *ReadXML*, but reads indexed file names.

The variable is read from a file with name:
   <filename>.<file_index>.xml.
where <file_index> is the value of *file_index*.

This means that *filename* shall here not include the .xml
extension. Omitting filename works as for *ReadXML*.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Workspace variable to be read.
@param[in] filename - File name. See above. (default: "")
@param[in] digits - Equalize the widths of all numbers by padding with zeros as
necessary. 0 means no padding (default). (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void ReadXMLIndexed(Workspace& ws, std::pair<Any_0, String>& out,
                    const std::pair<String, String>& filename = {"",
                                                                 "filename"},
                    const std::pair<Index, String>& digits = {0, "digits"}) {
  ReadXMLIndexed(out.first, out.second, Var::file_index(ws), filename.first,
                 digits.first, filename.second, digits.second,
                 Var::verbosity(ws));
}

/*! Reduces a larger class to a smaller class of same size.

The Reduce command reduces all "1"-dimensions to nil.  Examples:
        1) 1 Vector can be reduced to a Numeric
        2) 2x1 Matrix can be reduced to 2 Vector
        3) 1x3x1 Tensor3 can be reduced to 3 Vector
        4) 1x1x1x1 Tensor4 can be reduced to a Numeric
        5) 3x1x4x1x5 Tensor5 can only be reduced to 3x4x5 Tensor3
        6) 1x1x1x1x2x3 Tensor6 can be reduced to 2x3 Matrix
        7) 2x3x4x5x6x7x1 Tensor7 can be reduced to 2x3x4x5x6x7 Tensor6
And so on

@author Oliver Lemke
@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] o - Reduced form of input.
@param[in] i - Over-dimensioned input

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0, typename Any_1>
void Reduce(Workspace& ws, Any_0& o, const Any_1& i) {
  Reduce(o, i, Var::verbosity(ws));
}

/*! Adds single scattering data and particle number density for
individual scattering elements.

The methods reads the specified files and appends the obtained data
to *scat_data* and *pnd_field_raw*. Scattering data is appended to
the current last existing scattering species in *scat_data*.

@author Claudia Emde, Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] scat_data_files - List of names of single scattering data files.
@param[in] pnd_field_files - List of names of the corresponding pnd_field files.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ScatElementsPndAndScatAdd(Workspace& ws,
                               const ArrayOfString& scat_data_files,
                               const ArrayOfString& pnd_field_files) {
  ScatElementsPndAndScatAdd(Var::scat_data_raw(ws), Var::pnd_field_raw(ws),
                            Var::atmosphere_dim(ws), scat_data_files,
                            pnd_field_files, Var::verbosity(ws));
}

/*! Allows to limit considered scattering elements according to size.

Scattering elements of a specified scattering species are removed
from *scat_data_raw* and *scat_meta*, i.e. removed from further
calculations, if their particle size exceeds the specified limits.
Specification of the scattering species is done by name matching the
scattering species name part of *scat_species* tag.
As size parameter, all size parameters reported by the meta data
can be used (see *scat_meta_single* for offered parameters and
their naming).

@author Daniel Kreyling, Oliver Lemke, Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] species - Species on which to apply size selection.
@param[in] sizeparam - Size parameter to apply for size selection.
@param[in] sizemin - Minimum size [m] of the scattering elements to consider
(default: 0.)
@param[in] sizemax - Maximum size [m] of the scattering elements to consider (if
negative, no max. limitation is applied). (default: -1.)
@param[in] tolerance - Relative numerical tolerance of size limit values.
(default: 1e-6)
@param[in] delim - Delimiter string of *scat_species* elements. (default: "-")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ScatElementsSelect(Workspace& ws, const String& species,
                        const String& sizeparam, const Numeric& sizemin = 0.,
                        const Numeric& sizemax = -1.,
                        const Numeric& tolerance = 1e-6,
                        const String& delim = "-") {
  ScatElementsSelect(Var::scat_data_raw(ws), Var::scat_meta(ws),
                     Var::scat_species(ws), species, sizeparam, sizemin,
                     sizemax, tolerance, delim, Var::verbosity(ws));
}

/*! Appends scattering elements to *abs_species* including reading
single scattering data and corresponding pnd field.

The methods reads the specified single scattering and pnd_field
data of individual scattering elements and appends the obtained data
to *scat_data* (appending to its last scattering species) and
*vmr_field_raw*. Per scattering element, it also appends one
instance of species 'particles' to *abs_species*.

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] scat_data_files - List of names of single scattering data files.
@param[in] pnd_field_files - List of names of the corresponding pnd_field files.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ScatElementsToabs_speciesAdd(Workspace& ws,
                                  const ArrayOfString& scat_data_files,
                                  const ArrayOfString& pnd_field_files) {
  ScatElementsToabs_speciesAdd(
      Var::scat_data_raw(ws), Var::vmr_field_raw(ws), Var::abs_species(ws),
      Var::propmat_clearsky_agenda_checked(ws),
      Var::abs_xsec_agenda_checked(ws), Var::atmosphere_dim(ws),
      Var::f_grid(ws), scat_data_files, pnd_field_files, Var::verbosity(ws));
}

/*! Extends valid temperature range of single scattering data.

The method allows to extend the temperature range of given single
scattering data by duplicating optical property data at the low
and/or high limits of the associated temperature grid. *T_low* and
*T_high* specify the temperature grid points that are added.
Extension is only performed if *T_low* is lower and *T_high* is
higher than the original lowest and highest temperatures,
respectively, and if the original data contains more than one
temperature grid point (i.e., when not assumed constant anyways).

The method is thought, e.g., for atmospheric ice falling into
atmospheric layers with temperatures above the melting point of
ice, where ambient and particle temperature deviate (as long as
frozen the ice temperature remains at the melting point
temperature). It is not internally checked, whether the original
data includes the melting point.
The method can be used in a wider sense. However, it remains in the
responsibility of the user to apply the method in a meaningful
sense and on meaningful single scattering data.

The temperature extension is applied on all scattering elements of
a scattering species. If *scat_species* is defined, *species* can
be used to select the species on which the extension shall be
applied comparing *species* with the scattering species name part
of *scat_species*. If no *species* is specified, the method is
applied on the current last existing scattering species in
*scat_data*. Through the latter the method can be applied for cases
when *scat_species* is not defined (e.g. when *pnd_field* data is
created externally instead of from hydrometeor fields

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] species - Scattering species to act on (see WSM description for
details). (default: "")
@param[in] scat_species_delim - Delimiter string of *scat_species* elements.
(default: "-")
@param[in] T_low - Temperature grid extension point at low temperature limit.
(default: -1.)
@param[in] T_high - Temperature grid extension point at high temperature limit.
(default: -1.)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ScatSpeciesExtendTemperature(Workspace& ws, const String& species = "",
                                  const String& scat_species_delim = "-",
                                  const Numeric& T_low = -1.,
                                  const Numeric& T_high = -1.) {
  ScatSpeciesExtendTemperature(Var::scat_data_raw(ws), Var::scat_species(ws),
                               species, scat_species_delim, T_low, T_high,
                               Var::verbosity(ws));
}

/*! Initializes the scattering species related data variables.

This method initializes the *scat_species* WSV, the variables that
will hold the raw optical properties and the raw particle number
distributions of the scattering elements (*scat_data_raw* and
*pnd_field_raw*, respectively) as well as the one holding the meta
information about the scattering elements (*scat_meta*).

This method has to be executed before WSM reading/adding to the
said variable, e.g. before *ScatSpeciesPndAndScatAdd*.

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ScatSpeciesInit(Workspace& ws) {
  ScatSpeciesInit(Var::scat_species(ws), Var::scat_data_raw(ws),
                  Var::scat_meta(ws), Var::scat_data_checked(ws),
                  Var::pnd_field_raw(ws), Var::verbosity(ws));
}

/*! Merges single scattering data of all scattering elements into one
element of bulk properties.

Before entering the scattering solver, this method prepares the
effective bulk single scattering properties of all scattering
elements. Done by calculating the particle number density weighted
sum of the single scattering properties of all scattering elements
per pressure level. Accordingly, *pnd_field* is resized to
[np, np, 1, 1], where np is the number of pressure levels inside
the cloudbox. The diagonal elements of the new *pnd_field* are set
to 1, all others to 0. *scat_data* is resized to np. Each new
scattering element represents the weighted sum of all particles at
one presssure level.

The method also adapts *scat_species* and *scat_meta* such that
they remain consistent with *pnd_field* and can pass
*cloudbox_checkedCalc*.

The method is suggested to be called directly after
*pnd_fieldCalcFromParticleBulkProps* (but also after
*cloudbox_checkedCalc*).
Its purpose is to speed up the scattering calculations.

This is an experimental method currently only working for limited
cases. All scattering elements must be of the same ptype and must
share the same *f_grid*, *za_grid*, and *aa_grid*. That is, the
scattering matrix, extinction matrix, and absorption vector of all
scattering elements must have the same dimensions. No interpolation
(apart from temperature) is performed.

This method can only be used with a 1D atmosphere.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ScatSpeciesMerge(Workspace& ws) {
  ScatSpeciesMerge(Var::pnd_field(ws), Var::scat_data(ws), Var::scat_meta(ws),
                   Var::scat_species(ws), Var::cloudbox_checked(ws),
                   Var::atmosphere_dim(ws), Var::cloudbox_on(ws),
                   Var::cloudbox_limits(ws), Var::t_field(ws), Var::z_field(ws),
                   Var::z_surface(ws), Var::verbosity(ws));
}

/*! Adds single scattering data and particle number densities for one
scattering species.

The WSV *pnd_field_raw* containing particle number densities for
all scattering species can be generated outside ARTS, for example
by using PyARTS or atmlab. This method reads this data as well as
its corresponding single scattering data, which is added as a new
scattering species to *scat_data*.
This method needs as input an ArrayOfString holding the filenames
of the single scattering data for each scattering element and a
file containing the corresponding *pnd_field_raw*. In contrast to
the scattering data, the pnd-fields are stored in a single XML-file
containing an ArrayofGriddedField3, i.e. holding the pnd-field data
of all scattering elements.

Important note:
The order of the filenames for the scattering data files has to
correspond to the order of the pnd-fields, stored in the variable
*pnd_field_raw*.

@author Claudia Emde, Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] scat_data_files - Array of names of files containing the single
scattering data.
@param[in] pnd_fieldarray_file - Name of file holding the corresponding array of
pnd_field data.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ScatSpeciesPndAndScatAdd(Workspace& ws,
                              const ArrayOfString& scat_data_files,
                              const String& pnd_fieldarray_file) {
  ScatSpeciesPndAndScatAdd(Var::scat_data_raw(ws), Var::pnd_field_raw(ws),
                           Var::atmosphere_dim(ws), scat_data_files,
                           pnd_fieldarray_file, Var::verbosity(ws));
}

/*! Reads single scattering data and scattering meta data for one
scattering species.

This method takes a string array as input containing the location
(path and filename) of the single scattering data. Location of
corresponding scattering meta data is derived applying a naming
convention: ending '.xml*' is replaced by '.meta.xml' (search for
zipped files is done automatically).

All scattering elements read in one call of the method are assigned
to one and the same scattering species. That is, reading in data for
a bunch of scattering species can be realized by multiple call of
this method. Assignment to scattering species is in the order of the
calls (i.e., first method call reads data for first *scat_species*
entry, second call for second scat_species entry and so on).
Note that no two scattering elements of the same scattering species
are allowed to be equal in size*

Important note:
The order of the filenames for the single scattering data files has to
exactly correspond to the order of the scattering meta data files.

@author Daniel Kreyling, Oliver Lemke, Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] scat_data_files - Array of single scattering data file names.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ScatSpeciesScatAndMetaRead(Workspace& ws,
                                const ArrayOfString& scat_data_files) {
  ScatSpeciesScatAndMetaRead(Var::scat_data_raw(ws), Var::scat_meta(ws),
                             scat_data_files, Var::verbosity(ws));
}

/*! Derives size and mass information for a scattering species.
This method assumes that the mass-size relationship can described
by *scat_species_a* and *scat_species_b*. See documentation of
*scat_species_a* for details.

The quantity to be used as size descriptor is here denoted as x, and
is selected by setting *x_unit*. The options are:
 "dveq" : The size grid is set to scat_meta.diameter_volume_equ
 "dmax" : The size grid is set to scat_meta.diameter_max
 "area" : The size grid is set to scat_meta.diameter_area_equ_aerodynamical
 "mass" : The size grid is set to scat_meta.mass
This selection determines *scat_species_x*.

The parameters *scat_species_a* and *scat_species_b* are determined by
a numeric fit between *scat_species_x* and corresponding masses in
*scat_meta*. This fit is performed over sizes inside the range
[x_fit_start,x_fit_end]. This range is allowed to be broader than
the coverage of *scat_species_x*. There must be at least two sizes
inside [x_fit_start,x_fit_end].

@author Manfred Brath
@author Jana Mendrok
@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] species_index - Take data from scattering species of this index
(0-based) in *scat_meta*.
@param[in] x_unit - Unit for size grid, allowed options listed above.
@param[in] x_fit_start - Smallest size to consider in fit to determine a and b.
(default: 0)
@param[in] x_fit_end - Largest size to consider in fit to determine a and b.
(default: 1e9)
@param[in] do_only_x - A flag to deactivate calculation of a and b, to possibly
save some time. The a and b parameters are then set to -1.Default is to
calculate a and b. (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ScatSpeciesSizeMassInfo(Workspace& ws, const Index& species_index,
                             const String& x_unit,
                             const Numeric& x_fit_start = 0,
                             const Numeric& x_fit_end = 1e9,
                             const Index& do_only_x = 0) {
  ScatSpeciesSizeMassInfo(Var::scat_species_x(ws), Var::scat_species_a(ws),
                          Var::scat_species_b(ws), Var::scat_meta(ws),
                          species_index, x_unit, x_fit_start, x_fit_end,
                          do_only_x, Var::verbosity(ws));
}

/*! Creates a variable of group ScatteringMetaData.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ScatteringMetaDataCreate(Workspace& ws, ScatteringMetaData& out) {
  ScatteringMetaDataCreate(out, Var::verbosity(ws));
}

/*! Method to select some elements from one array and copy them to
a new array. (Works also for vectors.)

This works also for higher dimensional objects, where the selection is
always performed in the first dimension.

If needleindexes is set to [-1], all elements are copied.
For example:

Select(y,x,[0,3])

will select the first and fourth row of matrix x and copy them to the
output matrix y.

Note that it is even safe to use this method if needles and haystack
are the same variable.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] needles - Selected elements. Must have the same variable type as
haystack.
@param[in] haystack - Variable to select from. May be the same variable as
needles.
@param[in] needleindexes - The elements to select (zero based indexing, as
always.)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0, typename Any_1>
void Select(Workspace& ws, Any_0& needles, const Any_1& haystack,
            const ArrayOfIndex& needleindexes) {
  Select(needles, haystack, needleindexes, Var::verbosity(ws));
}

/*! Change the number of threads used by ARTS.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] nthreads - Number of threads.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void SetNumberOfThreads(Workspace& ws, const Index& nthreads) {
  SetNumberOfThreads(nthreads, Var::verbosity(ws));
}

/*! Creates a variable of group SingleScatteringData.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void SingleScatteringDataCreate(Workspace& ws, SingleScatteringData& out) {
  SingleScatteringDataCreate(out, Var::verbosity(ws));
}

/*! Sleeps for a number of seconds

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] time - Time to sleep for in seconds

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Sleep(Workspace& ws, const Numeric& time) {
  Sleep(time, Var::verbosity(ws));
}

/*! Creates a variable of group Sparse.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void SparseCreate(Workspace& ws, Sparse& out) {
  SparseCreate(out, Var::verbosity(ws));
}

/*! Returns a sparse dentity matrix.

The size of the matrix created is n x n. Default is to return a
true identity matrix (I), but you can also select another value
along the diagonal be setting *value*. That is, the output is
value*I.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Sparse output matrix
@param[in] n - Size of the matrix
@param[in] value - The value along the diagonal. (default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void SparseMatrixIdentity(Workspace& ws, Sparse& out, const Index& n,
                          const Numeric& value = 1) {
  SparseMatrixIdentity(out, n, value, Var::verbosity(ws));
}

/*! Multiplies a Sparse with another Sparse, result stored in Sparse.

Makes the calculation: out = m1 * m2

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Product, can be same variable as any of the inputs.
@param[in] m1 - Left sparse matrix.
@param[in] m2 - Right sparse matrix.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void SparseSparseMultiply(Workspace& ws, Sparse& out, const Sparse& m1,
                          const Sparse& m2) {
  SparseSparseMultiply(out, m1, m2, Var::verbosity(ws));
}

/*! Creates a variable of group SpeciesAuxData.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void SpeciesAuxDataCreate(Workspace& ws, SpeciesAuxData& out) {
  SpeciesAuxDataCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group StokesVector.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void StokesVectorCreate(Workspace& ws, StokesVector& out) {
  StokesVectorCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group String.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void StringCreate(Workspace& ws, String& out) {
  StringCreate(out, Var::verbosity(ws));
}

/*! Concatenate two or more strings.

The output string is overwritten, but is allowed to appear
in the input list. Up to 10 strings can be concatenated at once.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Concatenated string.
@param[in] in1 - Input text string.
@param[in] in2 - Input text string.
@param[in] in3 - Input text string. (default: "")
@param[in] in4 - Input text string. (default: "")
@param[in] in5 - Input text string. (default: "")
@param[in] in6 - Input text string. (default: "")
@param[in] in7 - Input text string. (default: "")
@param[in] in8 - Input text string. (default: "")
@param[in] in9 - Input text string. (default: "")
@param[in] in10 - Input text string. (default: "")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void StringJoin(Workspace& ws, String& out, const String& in1,
                const String& in2, const String& in3 = "",
                const String& in4 = "", const String& in5 = "",
                const String& in6 = "", const String& in7 = "",
                const String& in8 = "", const String& in9 = "",
                const String& in10 = "") {
  StringJoin(out, in1, in2, in3, in4, in5, in6, in7, in8, in9, in10,
             Var::verbosity(ws));
}

/*! Sets a String to the given text string.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to initialize.
@param[in] text - Input text string.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void StringSet(Workspace& ws, String& out, const String& text) {
  StringSet(out, text, Var::verbosity(ws));
}

/*! Dummy method for *iy_surface_agenda*.

If you don't make use of *surface_props_data* and associated
variables, include this method *iy_surface_agenda*. The method
just checks that the variables of concern are set to be empty,
and you don't need to include calls of *Ignore* and *Touch* in
the agenda.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void SurfaceDummy(Workspace& ws) {
  SurfaceDummy(Var::dsurface_rmatrix_dx(ws), Var::dsurface_emission_dx(ws),
               Var::atmosphere_dim(ws), Var::lat_grid(ws), Var::lon_grid(ws),
               Var::surface_props_data(ws), Var::surface_props_names(ws),
               Var::dsurface_names(ws), Var::jacobian_do(ws),
               Var::verbosity(ws));
}

/*! FASTEM sea surface microwave emissivity parametrization.

The variable *surface_props_data* must contain these data:
  "Water skin temperature"
  "Wind speed"
  "Wind direction"
  "Salinity"

For some details and comments see *FastemStandAlone* and *surfaceFastem*.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] transmittance - Transmittance along path of downwelling radiation. A
vector with the same length as *f_grid*.
@param[in] fastem_version - The version of FASTEM to use. (default: 6)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void SurfaceFastem(Workspace& ws, const Vector& transmittance,
                   const Index& fastem_version = 6) {
  SurfaceFastem(Var::surface_los(ws), Var::surface_rmatrix(ws),
                Var::dsurface_rmatrix_dx(ws), Var::surface_emission(ws),
                Var::dsurface_emission_dx(ws), Var::stokes_dim(ws),
                Var::atmosphere_dim(ws), Var::lat_grid(ws), Var::lon_grid(ws),
                Var::f_grid(ws), Var::rtp_pos(ws), Var::rtp_los(ws),
                Var::surface_props_data(ws), Var::surface_props_names(ws),
                Var::dsurface_names(ws), Var::jacobian_do(ws), transmittance,
                fastem_version, Var::verbosity(ws));
}

/*! TESSEM sea surface microwave emissivity parametrization.

The variable *surface_props_data* must contain these data:
  "Water skin temperature"
  "Wind speed"
  "Salinity"

This method computes surface emissivity and reflectivity matrices for
ocean surfaces using the TESSEM emissivity model: Prigent, C., et al.
Sea-surface emissivity parametrization from microwaves to millimetre
waves, QJRMS, 2017, 143.702: 596-605.

The validity range of the parametrization of is 10 to 700 GHz, but for
some extra flexibility frequencies between 5 and 900 GHz are accepted.
The accepted temperaute range for water skin temperature is
[260.0 K, 373.0 K]. Salinity shall be in the range [0,1]. That is, a
salinity of 3% is given as 0.03.

The model itself is represented by the neural networks in
*tessem_neth* and *tessem_netv*.

@author Simon Pfreundschuh
@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void SurfaceTessem(Workspace& ws) {
  SurfaceTessem(Var::surface_los(ws), Var::surface_rmatrix(ws),
                Var::dsurface_rmatrix_dx(ws), Var::surface_emission(ws),
                Var::dsurface_emission_dx(ws), Var::stokes_dim(ws),
                Var::atmosphere_dim(ws), Var::lat_grid(ws), Var::lon_grid(ws),
                Var::f_grid(ws), Var::rtp_pos(ws), Var::rtp_los(ws),
                Var::tessem_neth(ws), Var::tessem_netv(ws),
                Var::surface_props_data(ws), Var::surface_props_names(ws),
                Var::dsurface_names(ws), Var::jacobian_do(ws),
                Var::verbosity(ws));
}

/*! T-Matrix validation test.

Executes the standard test included with the T-Matrix Fortran code.
Should give the same as running the tmatrix_lp executable in
3rdparty/tmatrix/.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void TMatrixTest(Workspace& ws) { TMatrixTest(Var::verbosity(ws)); }

/*! Finds the tangent point of a propagation path.

The tangent point is here defined as the point with the lowest
altitude (which differes from the definition used in the code
where it is the point with the lowest radius, or equally the point
with a zenith angle of 90 deg.)

The tangent point is returned as a vector, with columns matching
e.g. *rte_pos*. If the propagation path has no tangent point, the
vector is set to NaN.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] tan_pos - The position vector of the tangent point.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void TangentPointExtract(Workspace& ws, Vector& tan_pos) {
  TangentPointExtract(tan_pos, Var::ppath(ws), Var::verbosity(ws));
}

/*! Prints information about the tangent point of a propagation path.

The tangent point is here defined as the point with the lowest
altitude (which differes from the definition used in the code
where it is the point with the lowest radius, or equally the point
with a zenith angle of 90 deg.)

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] level - Output level to use. (default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void TangentPointPrint(Workspace& ws, const Index& level = 1) {
  TangentPointPrint(Var::ppath(ws), level, Var::verbosity(ws));
}

/*! Creates a variable of group TelsemAtlas.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void TelsemAtlasCreate(Workspace& ws, TelsemAtlas& out) {
  TelsemAtlasCreate(out, Var::verbosity(ws));
}

/*! Adds a scalar value to all elements of a tensor3.

The result can either be stored in the same or another
variable.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Output tensor.
@param[in] in - Input tensor.
@param[in] value - The value to be added to the tensor.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Tensor3AddScalar(Workspace& ws, Tensor3& out, const Tensor3& in,
                      const Numeric& value) {
  Tensor3AddScalar(out, in, value, Var::verbosity(ws));
}

/*! Creates a variable of group Tensor3.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Tensor3Create(Workspace& ws, Tensor3& out) {
  Tensor3Create(out, Var::verbosity(ws));
}

/*! Extracts a Tensor3 from a Tensor4.

Copies book, page, row or column with given Index from input Tensor4
variable to output Tensor3.
Higher order equivalent of *VectorExtractFromMatrix*.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Extracted tensor.
@param[in] in - Input Tensor4.
@param[in] i - Index of book, page, row or column to extract.
@param[in] direction - Direction. "book" or "page" or "row" or "column".

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Tensor3ExtractFromTensor4(Workspace& ws, Tensor3& out, const Tensor4& in,
                               const Index& i, const String& direction) {
  Tensor3ExtractFromTensor4(out, in, i, direction, Var::verbosity(ws));
}

/*! Scales all elements of a tensor with the specified value.

The result can either be stored in the same or another
variable.

@author Mattias Ekstrom

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Output tensor.
@param[in] in - Input tensor.
@param[in] value - The value to be multiplied with the tensor.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Tensor3Scale(Workspace& ws, Tensor3& out, const Tensor3& in,
                  const Numeric& value) {
  Tensor3Scale(out, in, value, Var::verbosity(ws));
}

/*! Creates a tensor and sets all elements to the specified value.

The size is determined by *ncols*, *nrows* etc.

@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to initialize.
@param[in] value - Tensor value.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Tensor3SetConstant(Workspace& ws, Tensor3& out, const Numeric& value) {
  Tensor3SetConstant(out, Var::npages(ws), Var::nrows(ws), Var::ncols(ws),
                     value, Var::verbosity(ws));
}

/*! Adds a scalar value to all elements of a tensor4.

The result can either be stored in the same or another
variable.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Output tensor.
@param[in] in - Input tensor.
@param[in] value - The value to be added to the tensor.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Tensor4AddScalar(Workspace& ws, Tensor4& out, const Tensor4& in,
                      const Numeric& value) {
  Tensor4AddScalar(out, in, value, Var::verbosity(ws));
}

/*! Creates a variable of group Tensor4.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Tensor4Create(Workspace& ws, Tensor4& out) {
  Tensor4Create(out, Var::verbosity(ws));
}

/*! Scales all elements of a tensor with the specified value.

The result can either be stored in the same or another
variable.

@author Mattias Ekstrom

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Output tensor.
@param[in] in - Input tensor.
@param[in] value - The value to be multiplied with the tensor.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Tensor4Scale(Workspace& ws, Tensor4& out, const Tensor4& in,
                  const Numeric& value) {
  Tensor4Scale(out, in, value, Var::verbosity(ws));
}

/*! Creates a tensor and sets all elements to the specified value.

The size is determined by *ncols*, *nrows* etc.

@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to initialize.
@param[in] value - Tensor value.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Tensor4SetConstant(Workspace& ws, Tensor4& out, const Numeric& value) {
  Tensor4SetConstant(out, Var::nbooks(ws), Var::npages(ws), Var::nrows(ws),
                     Var::ncols(ws), value, Var::verbosity(ws));
}

/*! Creates a variable of group Tensor5.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Tensor5Create(Workspace& ws, Tensor5& out) {
  Tensor5Create(out, Var::verbosity(ws));
}

/*! Scales all elements of a tensor with the specified value.

The result can either be stored in the same or another
variable.

@author Mattias Ekstrom

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Output tensor.
@param[in] in - Input tensor.
@param[in] value - The value to be multiplied with the tensor.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Tensor5Scale(Workspace& ws, Tensor5& out, const Tensor5& in,
                  const Numeric& value) {
  Tensor5Scale(out, in, value, Var::verbosity(ws));
}

/*! Creates a tensor and sets all elements to the specified value.

The size is determined by *ncols*, *nrows* etc.

@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to initialize.
@param[in] value - Tensor value.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Tensor5SetConstant(Workspace& ws, Tensor5& out, const Numeric& value) {
  Tensor5SetConstant(out, Var::nshelves(ws), Var::nbooks(ws), Var::npages(ws),
                     Var::nrows(ws), Var::ncols(ws), value, Var::verbosity(ws));
}

/*! Creates a variable of group Tensor6.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Tensor6Create(Workspace& ws, Tensor6& out) {
  Tensor6Create(out, Var::verbosity(ws));
}

/*! Scales all elements of a tensor with the specified value.

The result can either be stored in the same or another
variable.

@author Mattias Ekstrom

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Output tensor.
@param[in] in - Input tensor.
@param[in] value - The value to be multiplied with the tensor.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Tensor6Scale(Workspace& ws, Tensor6& out, const Tensor6& in,
                  const Numeric& value) {
  Tensor6Scale(out, in, value, Var::verbosity(ws));
}

/*! Creates a tensor and sets all elements to the specified value.

The size is determined by *ncols*, *nrows* etc.

@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to initialize.
@param[in] value - Tensor value.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Tensor6SetConstant(Workspace& ws, Tensor6& out, const Numeric& value) {
  Tensor6SetConstant(out, Var::nvitrines(ws), Var::nshelves(ws),
                     Var::nbooks(ws), Var::npages(ws), Var::nrows(ws),
                     Var::ncols(ws), value, Var::verbosity(ws));
}

/*! Creates a variable of group Tensor7.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Tensor7Create(Workspace& ws, Tensor7& out) {
  Tensor7Create(out, Var::verbosity(ws));
}

/*! Scales all elements of a tensor with the specified value.

The result can either be stored in the same or another
variable.

@author Mattias Ekstrom

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Output tensor.
@param[in] in - Input tensor.
@param[in] value - The value to be multiplied with the tensor.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Tensor7Scale(Workspace& ws, Tensor7& out, const Tensor7& in,
                  const Numeric& value) {
  Tensor7Scale(out, in, value, Var::verbosity(ws));
}

/*! Creates a tensor and sets all elements to the specified value.

The size is determined by *ncols*, *nrows* etc.

@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to initialize.
@param[in] value - Tensor value.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Tensor7SetConstant(Workspace& ws, Tensor7& out, const Numeric& value) {
  Tensor7SetConstant(out, Var::nlibraries(ws), Var::nvitrines(ws),
                     Var::nshelves(ws), Var::nbooks(ws), Var::npages(ws),
                     Var::nrows(ws), Var::ncols(ws), value, Var::verbosity(ws));
}

/*! Creates a variable of group TessemNN.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void TessemNNCreate(Workspace& ws, TessemNN& out) {
  TessemNNCreate(out, Var::verbosity(ws));
}

/*! Reads the initialization data for the TESSEM NeuralNet from an ASCII file.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] tessem_nn - Tessem NeuralNet configuration.
@param[in] filename - NeuralNet parameters file as provided in the TESSEM 2
distribution.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void TessemNNReadAscii(Workspace& ws, TessemNN& tessem_nn,
                       const String& filename) {
  TessemNNReadAscii(tessem_nn, filename, Var::verbosity(ws));
}

/*! A dummy method that can be used for test purposes.

This method can be used by ARTS developers to quickly test stuff.
The implementation is in file m_general.cc. This just saves you the
trouble of adding a dummy method everytime you want to try
something out quickly.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Test(Workspace& ws) { Test(Var::verbosity(ws)); }

/*! A method that is used for the TestArrayOfAgenda test case.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] index - Index of agenda in array to execute. (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void TestArrayOfAgenda(Workspace& ws, const Index& index = 0) {
  TestArrayOfAgenda(ws, Var::test_agenda_array(ws), index, Var::verbosity(ws));
}

/*! Example method for TESSEM2.

When using the default neural network parameter files
from the Tessem 2 distribution, the input Vector should contain
5 elements:
   - Frequency (10-700) in GHz.
   - Theta (0-90) Incidence angle in degrees.
   - Windspeed (0-25) at 10m (m/s)
     Higher wind speed can be used, but without garantee.
   - Surface skin temperature (270-310) in K.
   - Salinity (0-0.04) in kg/kg

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] outvalues - Tessem output emissivity.
@param[in] net - Tessem NeuralNet parameters.
@param[in] invalues - Input data.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void TestTessem(Workspace& ws, Vector& outvalues, const TessemNN& net,
                const Vector& invalues) {
  TestTessem(outvalues, net, invalues, Var::verbosity(ws));
}

/*! Creates a variable of group Time.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void TimeCreate(Workspace& ws, Time& out) {
  TimeCreate(out, Var::verbosity(ws));
}

/*! Creates a variable of group Timer.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void TimerCreate(Workspace& ws, Timer& out) {
  TimerCreate(out, Var::verbosity(ws));
}

/*! As *Ignore* but for agenda output.

This method is handy for use in agendas in order to suppress
warnings about not-produced output workspace variables.

What it does, in case the variable is initialized already, is:
Nothing!
In case the variable is not yet initialized, it is set to NaN.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] in - Variable to do nothing with.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void Touch(Workspace& ws, Any_0& in) {
  Touch(in, Var::verbosity(ws));
}

/*! Creates a variable of group TransmissionMatrix.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void TransmissionMatrixCreate(Workspace& ws, TransmissionMatrix& out) {
  TransmissionMatrixCreate(out, Var::verbosity(ws));
}

/*! Adds a scalar to all elements of a vector.

The result can either be stored in the same or another vector.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Output vector
@param[in] in - Input vector.
@param[in] value - The value to be added to the vector.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void VectorAddScalar(Workspace& ws, Vector& out, const Vector& in,
                     const Numeric& value) {
  VectorAddScalar(out, in, value, Var::verbosity(ws));
}

/*! Element-wise addition of two vectors.

The method calculates c = a + b.

The variable *b* is allowed to have length 1, for any length of
*a*. This single value in *b* is then added to every element of *a*.

The vectors *a* and *c* can be the same WSV, while *b* can not be
the same WSV as any of the the other vector.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] c - Output vector
@param[in] a - Input vector.
@param[in] b - Vector to be added.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void VectorAddVector(Workspace& ws, Vector& c, const Vector& a,
                     const Vector& b) {
  VectorAddVector(c, a, b, Var::verbosity(ws));
}

/*! Creates a variable of group Vector.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void VectorCreate(Workspace& ws, Vector& out) {
  VectorCreate(out, Var::verbosity(ws));
}

/*! Keeps only values of a vector inside the specified range.

All values outside the range [min_value,max-value] are removed.
Note the default values, that basically should act as -+Inf.

The result can either be stored in the same or another vector.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Cropped vector
@param[in] in - Original vector
@param[in] min_value - Minimum value to keep (default: -99e99)
@param[in] max_value - Maximum value to keep (default: 99e99)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void VectorCrop(Workspace& ws, Vector& out, const Vector& in,
                const Numeric& min_value = -99e99,
                const Numeric& max_value = 99e99) {
  VectorCrop(out, in, min_value, max_value, Var::verbosity(ws));
}

/*! Extracts a Vector from a Matrix.

Copies row or column with given Index from input Matrix variable
to create output Vector.

@author Patrick Eriksson, Oliver Lemke, Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Extracted vector.
@param[in] in - Input matrix.
@param[in] i - Index of row or column.
@param[in] direction - Direction. "row" or "column".

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void VectorExtractFromMatrix(Workspace& ws, Vector& out, const Matrix& in,
                             const Index& i, const String& direction) {
  VectorExtractFromMatrix(out, in, i, direction, Var::verbosity(ws));
}

/*! Flips a vector.

The output is the input vector in reversed order. The result can
either be stored in the same or another vector.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Output vector.
@param[in] in - Input vector.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void VectorFlip(Workspace& ws, Vector& out, const Vector& in) {
  VectorFlip(out, in, Var::verbosity(ws));
}

/*! Insert some additional points into a grid.

This method can for example be used to add line center frequencies to
a regular frequency grid. If the original grid is [1,2,3], and the
additional points are [2.2,2.4], the result will be [1,2,2.2,2.4,3].

It is assumed that the original grid is sorted, otherwise a runtime
error is thrown. The vector with the points to insert does not have to
be sorted. If some of the input points are already in the grid, these
points are not inserted again. New points outside the original grid are
appended at the appropriate end. Input vector and output vector can be
the same.

Generic output:
  Vector : The new grid vector.

Generic input:
  Vector : The original grid vector.
  Vector : The points to insert.

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - The new grid vector
@param[in] in - The original grid vector
@param[in] points - The points to insert

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void VectorInsertGridPoints(Workspace& ws, Vector& out, const Vector& in,
                            const Vector& points) {
  VectorInsertGridPoints(out, in, points, Var::verbosity(ws));
}

/*! Initializes a vector with linear spacing.

The first element equals always the start value, and the spacing
equals always the step value, but the last value can deviate from
the stop value. *step* can be both positive and negative.

The created vector is [start, start+step, start+2*step, ...]

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Output vector.
@param[in] start - Start value.
@param[in] stop - Maximum/minimum value of the end value
@param[in] step - Spacing of the vector.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void VectorLinSpace(Workspace& ws, Vector& out, const Numeric& start,
                    const Numeric& stop, const Numeric& step) {
  VectorLinSpace(out, start, stop, step, Var::verbosity(ws));
}

/*! Initializes a vector with logarithmic spacing.

The first element equals always the start value, and the spacing
equals always the step value, but note that the last value can
deviate from the stop value. The keyword step can be both positive
and negative.

Note, that although start has to be given in direct coordinates,
step has to be given in log coordinates.

Explicitly, the vector is:
 exp([ln(start), ln(start)+step, ln(start)+2*step, ...])

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to initialize.
@param[in] start - The start value. (Direct coordinates!)
@param[in] stop - The maximum value of the end value. (Direct coordinates!)
@param[in] step - The spacing of the vector. (Log coordinates!)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void VectorLogSpace(Workspace& ws, Vector& out, const Numeric& start,
                    const Numeric& stop, const Numeric& step) {
  VectorLogSpace(out, start, stop, step, Var::verbosity(ws));
}

/*! Multiply a Vector with a Matrix and store the result in another
Vector.

This just computes the normal Matrix-Vector product, y=M*x. It is ok
if input and output Vector are the same. This function is handy for
multiplying the H Matrix to spectra.

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - The result of the multiplication (dimension m).
@param[in] m - The Matrix to multiply (dimension mxn).
@param[in] v - The original Vector (dimension n).

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void VectorMatrixMultiply(Workspace& ws, Vector& out, const Matrix& m,
                          const Vector& v) {
  VectorMatrixMultiply(out, m, v, Var::verbosity(ws));
}

/*! Creates a vector with length *nelem*, equally spaced between the
given end values.

The length (*nelem*) must be larger than 1.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to initialize.
@param[in] start - Start value.
@param[in] stop - End value.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void VectorNLinSpace(Workspace& ws, Vector& out, const Numeric& start,
                     const Numeric& stop) {
  VectorNLinSpace(out, Var::nelem(ws), start, stop, Var::verbosity(ws));
}

/*! Creates a vector with length *nelem*, equally logarithmically
spaced between the given end values.

The length (*nelem*) must be larger than 1.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to initialize.
@param[in] start - Start value.
@param[in] stop - End value.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void VectorNLogSpace(Workspace& ws, Vector& out, const Numeric& start,
                     const Numeric& stop) {
  VectorNLogSpace(out, Var::nelem(ws), start, stop, Var::verbosity(ws));
}

/*! Converts a Matrix to a Vector.

The matrix is reshaped into a vector. That is, all elements of the matrix
are kept. The elements can be extracted both in column (default) and row
order. The ouput vector has the same length for both options.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Created vector.
@param[in] in - Input matrix.
@param[in] direction - Direction. "row" or "column". (default: "column")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void VectorReshapeMatrix(Workspace& ws, Vector& out, const Matrix& in,
                         const String& direction = "column") {
  VectorReshapeMatrix(out, in, direction, Var::verbosity(ws));
}

/*! Scales all elements of a vector with the same value.

The result can either be stored in the same or another vector.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Output vector.
@param[in] in - Input vector.
@param[in] value - Scaling value.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void VectorScale(Workspace& ws, Vector& out, const Vector& in,
                 const Numeric& value) {
  VectorScale(out, in, value, Var::verbosity(ws));
}

/*! Create a vector from the given list of numbers.

   VectorSet(p_grid, [1000, 100, 10] )
   Will create a p_grid vector with these three elements.

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to initialize.
@param[in] value - The vector elements.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void VectorSet(Workspace& ws, Vector& out, const Vector& value) {
  VectorSet(out, value, Var::verbosity(ws));
}

/*! Creates a vector and sets all elements to the specified value.

The vector length is determined by *nelem*.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to initialize.
@param[in] value - Vector value.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void VectorSetConstant(Workspace& ws, Vector& out, const Numeric& value) {
  VectorSetConstant(out, Var::nelem(ws), value, Var::verbosity(ws));
}

/*! Element-wise subtraction of two vectors.

The method calculates c = a - b.

The variable *b* is allowed to have length 1, for any length of
*a*. This single value in *b* is then added to every element of *a*.

The vectors *a* and *c* can be the same WSV, while *b* can not be
the same WSV as any of the the other vector.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] c - Output vector
@param[in] a - Input vector.
@param[in] b - Vector to be subtracted.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void VectorSubtractVector(Workspace& ws, Vector& c, const Vector& a,
                          const Vector& b) {
  VectorSubtractVector(c, a, b, Var::verbosity(ws));
}

/*! Multiply a Vector with another Vector and store result in a third one.

This is an element-wise multiplication. It is ok if output Vector is
the same as any of the input Vectors.

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - The result of the multiplication (dimension m).
@param[in] v1 - Original Vector #1 (dimension m).
@param[in] v2 - Original Vector #2 (dimension m).

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void VectorVectorMultiply(Workspace& ws, Vector& out, const Vector& v1,
                          const Vector& v2) {
  VectorVectorMultiply(out, v1, v2, Var::verbosity(ws));
}

/*! Converts a set of geometrical tangent altitudes to zenith angles.

The tangent altitudes are given to the function as a vector, which
are converted to a generic vector of zenith angles. The position of
the sensor is given by the WSV *sensor_pos*. The function works
only for 1D. The zenith angles are always set to be positive.

@author Patrick Eriksson
@author Mattias Ekstrom

@param[in,out] Workspace ws - An ARTS workspace
@param[out] v_za - Vector with zenith angles.
@param[in] v_ztan - Vector with tangent altitudes.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void VectorZtanToZa1D(Workspace& ws, Vector& v_za, const Vector& v_ztan) {
  VectorZtanToZa1D(v_za, Var::sensor_pos(ws), Var::refellipsoid(ws),
                   Var::atmosphere_dim(ws), v_ztan, Var::verbosity(ws));
}

/*! Converts a set of true tangent altitudes to zenith angles.

The tangent altitudes are given to the function as a vector, which
are converted to a generic vector of zenith angles. The position of
the sensor is given by the WSV *sensor_pos*. The function works
only for 1D. The zenith angles are always set to be positive.

@author Patrick Eriksson
@author Mattias Ekstrom

@param[in,out] Workspace ws - An ARTS workspace
@param[out] v_za - Vector with zenith angles.
@param[in] v_ztan - Vector with tangent altitudes.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void VectorZtanToZaRefr1D(Workspace& ws, Vector& v_za, const Vector& v_ztan) {
  VectorZtanToZaRefr1D(ws, v_za, Var::refr_index_air_agenda(ws),
                       Var::sensor_pos(ws), Var::p_grid(ws), Var::t_field(ws),
                       Var::z_field(ws), Var::vmr_field(ws),
                       Var::refellipsoid(ws), Var::atmosphere_dim(ws),
                       Var::f_grid(ws), v_ztan, Var::verbosity(ws));
}

/*! Creates a variable of group Verbosity.

After being created, the variable is uninitialized.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Variable to create.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void VerbosityCreate(Workspace& ws, Verbosity& out) {
  VerbosityCreate(out, Var::verbosity(ws));
}

/*! Select some channels for WMRF calculation.

The HIRS fast setup consists of a precalculated frequency grid
covering all HIRS channels, and associated weights for each channel,
stored in a weight matrix. (A *sensor_response* matrix.)

If not all channels are requested for
simulation, then this method can be used to remove the unwanted
channels. It changes a number of variables in consistent fashion:

- Unwanted channels are removed from f_backend.
- Unwanted channels are removed from wmrf_weights.
- Unnecessary frequencies are removed from f_grid.
- Unnecessary frequencies are removed from wmrf_weights.

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void WMRFSelectChannels(Workspace& ws) {
  WMRFSelectChannels(Var::f_grid(ws), Var::wmrf_weights(ws), Var::f_backend(ws),
                     Var::wmrf_channels(ws), Var::verbosity(ws));
}

/*! Initialize the wigner 3 tables

The default values take about 400 Mb memory.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] fast_wigner_stored_symbols - Number of stored symbols possible before
replacements (default: 20000000)
@param[in] largest_wigner_symbol_parameter - Largest symbol used for
initializing factorials (e.g., largest J or L) (default: 250)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Wigner3Init(Workspace& ws,
                 const Index& fast_wigner_stored_symbols = 20000000,
                 const Index& largest_wigner_symbol_parameter = 250) {
  Wigner3Init(Var::wigner_initialized(ws), fast_wigner_stored_symbols,
              largest_wigner_symbol_parameter, Var::verbosity(ws));
}

/*! Unloads the wigner 3 tables

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Wigner3Unload(Workspace& ws) {
  Wigner3Unload(Var::wigner_initialized(ws), Var::verbosity(ws));
}

/*! Initialize the wigner 3 and 6 tables

The default values take about 1 Gb memory.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] fast_wigner_stored_symbols - Number of stored symbols possible before
replacements (default: 20000000)
@param[in] largest_wigner_symbol_parameter - Largest symbol used for
initializing factorials (e.g., largest J or L) (default: 250)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Wigner6Init(Workspace& ws,
                 const Index& fast_wigner_stored_symbols = 20000000,
                 const Index& largest_wigner_symbol_parameter = 250) {
  Wigner6Init(Var::wigner_initialized(ws), fast_wigner_stored_symbols,
              largest_wigner_symbol_parameter, Var::verbosity(ws));
}

/*! Unloads the wigner 3 and 6 tables

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void Wigner6Unload(Workspace& ws) {
  Wigner6Unload(Var::wigner_initialized(ws), Var::verbosity(ws));
}

/*! Prints the fast wigner table information if compiled with this option

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void WignerFastInfoPrint(Workspace& ws) {
  WignerFastInfoPrint(Var::wigner_initialized(ws), Var::verbosity(ws));
}

/*! Interpolation of raw wind fields to calculation grids.
Heritage from *AtmFieldsCalc*

Internally, *WindFieldsCalc* applies *GriddedFieldPRegrid* and
*GriddedFieldLatLonRegrid*. Generally, 'half-grid-step' extrapolation
is allowed and applied.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] interp_order - Interpolation order (1=linear interpolation).
(default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void WindFieldsCalc(Workspace& ws, const Index& interp_order = 1) {
  WindFieldsCalc(Var::wind_u_field(ws), Var::wind_v_field(ws),
                 Var::wind_w_field(ws), Var::p_grid(ws), Var::lat_grid(ws),
                 Var::lon_grid(ws), Var::wind_u_field_raw(ws),
                 Var::wind_v_field_raw(ws), Var::wind_w_field_raw(ws),
                 Var::atmosphere_dim(ws), interp_order, Var::verbosity(ws));
}

/*! Interpolation of 1D raw atmospheric fields to create 2D or 3D
homogeneous wind fields.  Derived from *AtmFieldsCalcExpand1D*

The method works as *WindFieldsCalc*, but accepts only raw 1D
wind fields. The raw data is interpolated to *p_grid* and
the obtained values are applied for all latitudes, and also
longitudes for 3D, to create a homogeneous atmosphere.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] interp_order - Interpolation order (1=linear interpolation).
(default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void WindFieldsCalcExpand1D(Workspace& ws, const Index& interp_order = 1) {
  WindFieldsCalcExpand1D(Var::wind_u_field(ws), Var::wind_v_field(ws),
                         Var::wind_w_field(ws), Var::p_grid(ws),
                         Var::lat_grid(ws), Var::lon_grid(ws),
                         Var::wind_u_field_raw(ws), Var::wind_v_field_raw(ws),
                         Var::wind_w_field_raw(ws), Var::atmosphere_dim(ws),
                         interp_order, Var::verbosity(ws));
}

/*! Reads wind field data from a scenario.

A full set of field components is read (NOTE: fails if scenario
only contains selected field components). The files can be
anywhere, but must all be in the same directory specified by
'basename'. Naming convention for the field component files is
basename.wind_u.xml for the u-component, v- and w-components
accordingly.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] basename - Name of scenario, probably including the full path. For
example: "/data/wind_field"

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void WindRawRead(Workspace& ws, const String& basename) {
  WindRawRead(Var::wind_u_field_raw(ws), Var::wind_v_field_raw(ws),
              Var::wind_w_field_raw(ws), basename, Var::verbosity(ws));
}

/*! Writes a 'molecular_tau_file' as required for libRadtran.

The libRadtran (www.libradtran.org) radiative transfer package is a
comprehensive package for various applications, it can be used to
compute radiances, irradiances, actinic fluxes, ... for the solar
and the thermal spectral ranges. Absorption is usually treated using
k-distributions or other parameterizations. For calculations with high
spectral resolution it requires absorption coefficients from an external
line-by-line model. Using this method, arts generates a file that can be
used by libRadtran (option molecular_tau_file).

@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace
@param[in] filename - Name of the *molecular_tau_file*.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void WriteMolTau(Workspace& ws, const String& filename) {
  WriteMolTau(Var::f_grid(ws), Var::z_field(ws),
              Var::propmat_clearsky_field(ws), Var::atmosphere_dim(ws),
              filename, Var::verbosity(ws));
}

/*! Writes a workspace variable to a NetCDF file.

This method can write variables of limited groups.

If the filename is omitted, the variable is written
to <basename>.<variable_name>.nc.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] in - Variable to be saved.
@param[in] filename - Name of the NetCDF file. (default: "")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void WriteNetCDF(Workspace& ws, const std::pair<Any_0, String>& in,
                 const std::pair<String, String>& filename = {"", "filename"}) {
  WriteNetCDF(in.first, filename.first, in.second, filename.second,
              Var::verbosity(ws));
}

/*! As *WriteNetCDF*, but creates indexed file names.

This method can write variables of any group.

If the filename is omitted, the variable is written
to <basename>.<variable_name>.nc.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] in - Variable to be saved.
@param[in] filename - Name of the NetCDF file. (default: "")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void WriteNetCDFIndexed(Workspace& ws, const std::pair<Any_0, String>& in,
                        const std::pair<String, String>& filename = {
                            "", "filename"}) {
  WriteNetCDFIndexed(Var::file_index(ws), in.first, filename.first, in.second,
                     filename.second, Var::verbosity(ws));
}

/*! Writes a workspace variable to an XML file.

This method can write variables of any group.

If the filename is omitted, the variable is written
to <basename>.<variable_name>.xml.
If no_clobber is set to 1, an increasing number will be
appended to the filename if the file already exists.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] in - Variable to be saved.
@param[in] filename - Name of the XML file. (default: "")
@param[in] no_clobber - 0: Overwrite existing files, 1: Use unique filenames
(default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void WriteXML(Workspace& ws, const std::pair<Any_0, String>& in,
              const std::pair<String, String>& filename = {"", "filename"},
              const std::pair<Index, String>& no_clobber = {0, "no_clobber"}) {
  WriteXML(Var::output_file_format(ws), in.first, filename.first,
           no_clobber.first, in.second, filename.second, no_clobber.second,
           Var::verbosity(ws));
}

/*! As *WriteXML*, but creates indexed file names.

The variable is written to a file with name:
   <filename>.<file_index>.xml.
where <file_index> is the value of *file_index*.

This means that *filename* shall here not include the .xml
extension. Omitting filename works as for *WriteXML*.

@author Patrick Eriksson, Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] in - Workspace variable to be saved.
@param[in] filename - File name. See above. (default: "")
@param[in] digits - Equalize the widths of all numbers by padding with zeros as
necessary. 0 means no padding (default). (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void WriteXMLIndexed(Workspace& ws, const std::pair<Any_0, String>& in,
                     const std::pair<String, String>& filename = {"",
                                                                  "filename"},
                     const std::pair<Index, String>& digits = {0, "digits"}) {
  WriteXMLIndexed(Var::output_file_format(ws), Var::file_index(ws), in.first,
                  filename.first, digits.first, in.second, filename.second,
                  digits.second, Var::verbosity(ws));
}

/*! Simple conversion from pressure to altitude.

This function converts a vector of pressure values to an approximate vector
of corresponding heights. The formula used to convert pressure to height is:
z = 16000 * (5.0 - log10(p))That is, a pressure is  assumed to decrease by a
factor of 10 every 16km.

Note that all pressure values in the vector must be greater than 0.01.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[out] z_grid - Approximate heights of pressure grid points.
@param[in] p_grid - Pressure grid.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ZFromPSimple(Workspace& ws, Vector& z_grid, const Vector& p_grid) {
  ZFromPSimple(z_grid, p_grid, Var::verbosity(ws));
}

/*! Takes CIARecord as input and appends the results in the appropriate place.

If CIARecord has same species as species in *abs_cia_data*, then the array
position is used to append all of the CIARecord into the array.  If clobber
evaluates as true, cia_record overwrites the appropriate *abs_cia_data*.  If
species in cia_record are not in *abs_cia_data*, the CIARecord is pushed back.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] cia_record - CIA record to append to *abs_cia_data*.
@param[in] clobber - If true, the new input clobbers the old cia data. (default:
0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_cia_dataAddCIARecord(Workspace& ws, const CIARecord& cia_record,
                              const Index& clobber = 0) {
  abs_cia_dataAddCIARecord(Var::abs_cia_data(ws), cia_record, clobber,
                           Var::verbosity(ws));
}

/*! Read data from a CIA data file for all CIA molecules defined
in *abs_species*.

The units in the HITRAN file are:
Frequency: cm^(-1)
Binary absorption cross-section: cm^5 molec^(-2)

Upon reading we convert this to the ARTS internal SI units
of Hz and m^5 molec^(-2).

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] catalogpath - Path to the CIA catalog directory.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_cia_dataReadFromCIA(Workspace& ws, const String& catalogpath) {
  abs_cia_dataReadFromCIA(Var::abs_cia_data(ws), Var::abs_species(ws),
                          catalogpath, Var::verbosity(ws));
}

/*! Read data from a CIA XML file and check that all CIA tags defined
in *abs_species* are present in the file.

The units of the data are described in *abs_cia_dataReadFromCIA*.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] filename - Name of the XML file. (default: "")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_cia_dataReadFromXML(Workspace& ws, const String& filename = "") {
  abs_cia_dataReadFromXML(Var::abs_cia_data(ws), Var::abs_species(ws), filename,
                          Var::verbosity(ws));
}

/*! Calculate absorption coefficients from cross sections.

This calculates both the total absorption and the
absorption per species.

Cross sections are multiplied by n*VMR.

@author Stefan Buehler
@author Axel von Engeln

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_coefCalcFromXsec(Workspace& ws) {
  abs_coefCalcFromXsec(
      Var::abs_coef(ws), Var::src_coef(ws), Var::dabs_coef_dx(ws),
      Var::dsrc_coef_dx(ws), Var::abs_coef_per_species(ws),
      Var::src_coef_per_species(ws), Var::abs_xsec_per_species(ws),
      Var::src_xsec_per_species(ws), Var::dabs_xsec_per_species_dx(ws),
      Var::dsrc_xsec_per_species_dx(ws), Var::abs_species(ws),
      Var::jacobian_quantities(ws), Var::abs_vmrs(ws), Var::abs_p(ws),
      Var::abs_t(ws), Var::verbosity(ws));
}

/*! Appends the description of a continuum model or a complete absorption
model to *abs_cont_names* and *abs_cont_parameters*.

See online documentation for *abs_cont_names* for a list of
allowed models and for information what parameters they require. See
file includes/continua.arts for default parameters for the various models.

@author Thomas Kuhn
@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace
@param[in] tagname - The name (species tag) of a continuum model. Must match one
of the models implemented in ARTS.

@param[in] model - A string selecting a particular continuum/full model under
this species tag.

@param[in] userparam - A Vector containing the required parameters for the
selected model. The meaning of the parameters and how many parameters are
required depends on the model. (default: {})

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_cont_descriptionAppend(Workspace& ws, const String& tagname,
                                const String& model,
                                const Vector& userparam = {}) {
  abs_cont_descriptionAppend(Var::abs_cont_names(ws), Var::abs_cont_models(ws),
                             Var::abs_cont_parameters(ws), tagname, model,
                             userparam, Var::verbosity(ws));
}

/*! Initializes the two workspace variables for the continuum description,
*abs_cont_names* and *abs_cont_parameters*.

This method does not really do anything, except setting the two
variables to empty Arrays. It is just necessary because the method
*abs_cont_descriptionAppend* wants to append to the variables.

Formally, the continuum description workspace variables are required
by the absorption calculation methods (e.g., *abs_coefCalcFromXsec*). Therefore
you always have to call at least *abs_cont_descriptionInit*, even if you do not
want to use any continua.

@author Thomas Kuhn
@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_cont_descriptionInit(Workspace& ws) {
  abs_cont_descriptionInit(Var::abs_cont_names(ws), Var::abs_cont_models(ws),
                           Var::abs_cont_parameters(ws), Var::verbosity(ws));
}

/*! Reads HITRAN line mixing data from a basedir
The basedir must point at line mixing data as provided by HITRAN.
The lines will be changed such that ALL CO2 lines are truncated
before adding the HITRAN line mixing lines.

The available modes are such that "VP*" uses Voigt profiles and
"SDVP*" uses speed-dependent Voigt profiles, where the "_Y"
signifies if Rosenkranz-style line mixing is considered or not, and
the "W" at the end signifies that full calculations are used.  At
the line mixing limit, line mixing is simply turned off.

The "FullW" mode uses Lorentzian calculations with the full relaxation
matrix until the line mixing limit is reached and it switches to Voigt.

The HITRAN LM data is available for download at:
https://hitran.org/supplementary/

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] basedir - Direcory where the linemixing data is to be found
@param[in] linemixinglimit - Line mixing limit as defined by *AbsorptionLines*
(default: -1)
@param[in] fmin - Minimum frequency to read from (default: 0)
@param[in] fmax - Maximum frequency to read until (default: 1e99)
@param[in] stot - Minimum integrated band strength to consider (default: 0)
@param[in] mode - Mode of calculations.  The options are: "VP", "VP_Y", "SDVP",
"SDVP_Y", "FullW", and "VP_W" (default: "VP_W")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_hitran_relmat_dataReadHitranRelmatDataAndLines(
    Workspace& ws, const String& basedir, const Numeric& linemixinglimit = -1,
    const Numeric& fmin = 0, const Numeric& fmax = 1e99,
    const Numeric& stot = 0, const String& mode = "VP_W") {
  abs_hitran_relmat_dataReadHitranRelmatDataAndLines(
      Var::abs_hitran_relmat_data(ws), Var::abs_lines_per_species(ws),
      Var::abs_species(ws), basedir, linemixinglimit, fmin, fmax, stot, mode,
      Var::verbosity(ws));
}

/*! Appends all lines in *abs_lines* that match with lines in replacement_lines
if *safe*. If not *safe*, appends all lines.

No appended line is allowed to match any line in *abs_lines* if *safe*

Conditional behavior if *safe*:
        If the AbosorptionLines to be appended match no AbsorptionLines
        in *abs_lines*, then the entire AbsorptionLines is appended.
        Otherwise, only a single AbsorptionLines can be matched and is not
        allowed to have any internal matches

Note that lines are identified by their AbsorptionLines tags and by their
quantum numbers in *safe* mode.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] appending_lines - Line-array that appends lines in *abs_lines*.
@param[in] safe - Flag whether to check quantum numbers or not (default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesAppendWithLines(Workspace& ws,
                              const ArrayOfAbsorptionLines& appending_lines,
                              const Index& safe = 1) {
  abs_linesAppendWithLines(Var::abs_lines(ws), appending_lines, safe,
                           Var::verbosity(ws));
}

/*! Change parameter of all levels in *abs_lines* that match with
*QuantumIdentifier*. Only works for these parameters: parameter_name =
"Statistical Weight" parameter_name = "Zeeman Coefficient"

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] QI - Information to match the level.
@param[in] parameter_name - Name of parameter to be replaced
@param[in] change - Value with which to change matching level's value
@param[in] relative - Flag for relative change (0 is absolute change) (default:
0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesChangeBaseParameterForMatchingLevel(Workspace& ws,
                                                  const QuantumIdentifier& QI,
                                                  const String& parameter_name,
                                                  const Numeric& change,
                                                  const Index& relative = 0) {
  abs_linesChangeBaseParameterForMatchingLevel(Var::abs_lines(ws), QI,
                                               parameter_name, change, relative,
                                               Var::verbosity(ws));
}

/*! See *abs_linesChangeBaseParameterForMatchingLevel*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] QI - Information to match the level.
@param[in] parameter_name - Name of parameter to be replaced
@param[in] change - Value with which to change matching level's value
@param[in] relative - Flag for relative change (0 is absolute change) (default:
0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesChangeBaseParameterForMatchingLevels(
    Workspace& ws, const ArrayOfQuantumIdentifier& QI,
    const String& parameter_name, const Vector& change,
    const Index& relative = 0) {
  abs_linesChangeBaseParameterForMatchingLevels(Var::abs_lines(ws), QI,
                                                parameter_name, change,
                                                relative, Var::verbosity(ws));
}

/*! Change parameter of all lines in *abs_lines* that match with
*QuantumIdentifier*. Only works for these parameters: parameter_name = "Central
Frequency" parameter_name = "Line Strength" parameter_name = "Lower State
Energy" parameter_name = "Einstein Coefficient" parameter_name = "Lower
Statistical Weight" parameter_name = "Upper Statistical Weight" parameter_name =
"Lower Zeeman Coefficient" parameter_name = "Upper Zeeman Coefficient"

Note that loose_matching:=0 means only identical quantum identifiers are
accepted, otherwise the numbers in QI must just be contained in the line
identifier

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] QI - Information to match the line.
@param[in] parameter_name - Name of parameter to be replaced
@param[in] change - Value with which to change matching line's value
@param[in] relative - Flag for relative change (0 is absolute change) (default:
0)
@param[in] loose_matching - Flag for loose match (0 means only complete matches)
(default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesChangeBaseParameterForMatchingLines(
    Workspace& ws, const QuantumIdentifier& QI, const String& parameter_name,
    const Numeric& change, const Index& relative = 0,
    const Index& loose_matching = 0) {
  abs_linesChangeBaseParameterForMatchingLines(
      Var::abs_lines(ws), QI, parameter_name, change, relative, loose_matching,
      Var::verbosity(ws));
}

/*! Change line shape model data parameter in matching lines.

The matching is done so that QI must be in the line identifier

Acceptable parameter(s) are:
        "G0"
        "D0"
        "G2"
        "D2"
        "FVC"
        "ETA"
        "Y"
        "G"
        "DV"

Acceptable coefficient(s) are:
        "X0"
        "X1"
        "X2"
        "X3"

Acceptable species are:
        "AIR" (so long as it is the broadening species list)
        "SELF" (so long as it is the broadening species list)
        Any species in the line broadening species

The line parameter will have its old value plus the change if
relative is false, else it will have its old value times
(1+change).

Throws an error if it cannot find any targets to change

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] QI - Information to match the line.
@param[in] parameter - Name of parameter to be replaced
@param[in] coefficient - Coefficient of the parameter to be changed
@param[in] species - Species of parameter to be changed
@param[in] change - Change in the value found
@param[in] relative - Flag for relative change (0 is absolute change) (default:
0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesChangeLineShapeModelParameterForMatchingLines(
    Workspace& ws, const QuantumIdentifier& QI, const String& parameter,
    const String& coefficient, const String& species, const Numeric& change,
    const Index& relative = 0) {
  abs_linesChangeLineShapeModelParameterForMatchingLines(
      Var::abs_lines(ws), QI, parameter, coefficient, species, change, relative,
      Var::verbosity(ws));
}

/*! Removes empty bands from *abs_lines*.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesCleanupEmpty(Workspace& ws) {
  abs_linesCleanupEmpty(Var::abs_lines(ws), Var::verbosity(ws));
}

/*! Removes lines that are unimportant because of their
cutoff frequency range

@author Stefan Buehler
@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesCompact(Workspace& ws) {
  abs_linesCompact(Var::abs_lines(ws), Var::f_grid(ws), Var::verbosity(ws));
}

/*! Deletes all lines in *abs_lines* that have bad central frequencies

If lower evaluates as true, deletes all lines with a frequency below f0.
Otherwise deletes all lines with a frequency above f0.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] f0 - Target frequency
@param[in] lower - Lower or upper flag (eval as boolean) (default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesDeleteBadF0(Workspace& ws, const Numeric& f0,
                          const Index& lower = 1) {
  abs_linesDeleteBadF0(Var::abs_lines(ws), f0, lower, Var::verbosity(ws));
}

/*! Deletes all lines in *abs_lines* that have undefined Js or Js
that change more than 1 between energy levels

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesDeleteLinesWithBadOrHighChangingJs(Workspace& ws) {
  abs_linesDeleteLinesWithBadOrHighChangingJs(Var::abs_lines(ws),
                                              Var::verbosity(ws));
}

/*! Deletes all lines in *abs_lines* that have too large quantum number

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] quantumnumber - Quantum number identified
@param[in] quantumnumber_value - Value

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesDeleteLinesWithQuantumNumberAbove(
    Workspace& ws, const String& quantumnumber,
    const Index& quantumnumber_value) {
  abs_linesDeleteLinesWithQuantumNumberAbove(Var::abs_lines(ws), quantumnumber,
                                             quantumnumber_value,
                                             Var::verbosity(ws));
}

/*! Deletes all lines in *abs_lines* that have undefined local quanta

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesDeleteLinesWithUndefinedLocalQuanta(Workspace& ws) {
  abs_linesDeleteLinesWithUndefinedLocalQuanta(Var::abs_lines(ws),
                                               Var::verbosity(ws));
}

/*! Deletes all lines in *abs_lines* that match with lines in replacement_lines.

If a deleted line has no match, then nothing happens.

Note that lines are identified by their AbsorptionLines tags and by their
quantum numbers. There is no need to have all values correct.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] deleting_lines - Line-array that removes lines from *abs_lines*.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesDeleteWithLines(Workspace& ws,
                              const ArrayOfAbsorptionLines& deleting_lines) {
  abs_linesDeleteWithLines(Var::abs_lines(ws), deleting_lines,
                           Var::verbosity(ws));
}

/*! Keep only *qid*-matches in *abs_lines*

The ignore values will ignore isotopologue and/or species.
The latter means the isotopologue has to be ignores.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] qid - Band ID
@param[in] ignore_spec - If species is to be ignores (default: 1)
@param[in] ignore_isot - If isotopologue is to be ignored (default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesKeepBands(Workspace& ws, const QuantumIdentifier& qid,
                        const Index& ignore_spec = 1,
                        const Index& ignore_isot = 1) {
  abs_linesKeepBands(Var::abs_lines(ws), qid, ignore_spec, ignore_isot,
                     Var::verbosity(ws));
}

/*! Print the count of defined quantum numbers in the catalog

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesPrintDefinedQuantumNumbers(Workspace& ws) {
  abs_linesPrintDefinedQuantumNumbers(Var::abs_lines(ws), Var::verbosity(ws));
}

/*! Reads a catalog of absorption lines files in a directory

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] basename - The path to the split catalog files
@param[in] robust - Flag to continue in case nothing is found [0 throws, 1
continues] (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesReadSpeciesSplitCatalog(Workspace& ws, const String& basename,
                                      const Index& robust = 0) {
  abs_linesReadSpeciesSplitCatalog(Var::abs_lines(ws), basename, robust,
                                   Var::verbosity(ws));
}

/*! Removes *qid* band from *abs_lines*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] qid - Band ID

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesRemoveBand(Workspace& ws, const QuantumIdentifier& qid) {
  abs_linesRemoveBand(Var::abs_lines(ws), qid, Var::verbosity(ws));
}

/*! Removes unused quantums from local values in the line lists

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesRemoveUnusedLocalQuantumNumbers(Workspace& ws) {
  abs_linesRemoveUnusedLocalQuantumNumbers(Var::abs_lines(ws),
                                           Var::verbosity(ws));
}

/*! Replace all lines in *abs_lines* that match with lines in replacement_lines.

Each replacement_lines must match excatly a single line in *abs_lines*.

The matching required identical quantum number signatures to work

Note that lines are identified by their AbsorptionLines tags and by their
quantum numbers.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] replacing_lines - Line-array that replace lines in *abs_lines*.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesReplaceWithLines(Workspace& ws,
                               const ArrayOfAbsorptionLines& replacing_lines) {
  abs_linesReplaceWithLines(Var::abs_lines(ws), replacing_lines,
                            Var::verbosity(ws));
}

/*! Set parameter of all levels in *abs_lines* that match with
*QuantumIdentifier*. Only works for these parameters: parameter_name =
"Statistical Weight" parameter_name = "Zeeman Coefficient"

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] QI - Information to match the level.
@param[in] parameter_name - Name of parameter to be replaced
@param[in] change - Value with which to set matching level's value

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesSetBaseParameterForMatchingLevel(Workspace& ws,
                                               const QuantumIdentifier& QI,
                                               const String& parameter_name,
                                               const Numeric& change) {
  abs_linesSetBaseParameterForMatchingLevel(
      Var::abs_lines(ws), QI, parameter_name, change, Var::verbosity(ws));
}

/*! See *abs_linesSetBaseParameterForMatchingLevel*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] QI - Information to match the level.
@param[in] parameter_name - Name of parameter to be replaced
@param[in] change - Value with which to set matching level's value

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesSetBaseParameterForMatchingLevels(
    Workspace& ws, const ArrayOfQuantumIdentifier& QI,
    const String& parameter_name, const Vector& change) {
  abs_linesSetBaseParameterForMatchingLevels(
      Var::abs_lines(ws), QI, parameter_name, change, Var::verbosity(ws));
}

/*! Set parameter of all lines in *abs_lines* that match with
*QuantumIdentifier*. Only works for these parameters: parameter_name = "Central
Frequency" parameter_name = "Line Strength" parameter_name = "Lower State
Energy" parameter_name = "Einstein Coefficient" parameter_name = "Lower
Statistical Weight" parameter_name = "Upper Statistical Weight" parameter_name =
"Lower Zeeman Coefficient" parameter_name = "Upper Zeeman Coefficient"

Note that loose_matching:=0 means only identical quantum identifiers are
accepted, otherwise the numbers in QI must just be contained in the line
identifier

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] QI - Information to match the line.
@param[in] parameter_name - Name of parameter to be replaced
@param[in] change - Value with which to change matching line's value
@param[in] loose_matching - Flag for loose match (0 means only complete matches)
(default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesSetBaseParameterForMatchingLines(
    Workspace& ws, const QuantumIdentifier& QI, const String& parameter_name,
    const Numeric& change, const Index& loose_matching = 0) {
  abs_linesSetBaseParameterForMatchingLines(Var::abs_lines(ws), QI,
                                            parameter_name, change,
                                            loose_matching, Var::verbosity(ws));
}

/*! Sets cutoff type and magnitude for all lines.

The line is cut off when this is active at the given frequency.
The only non-zero range is from this range to its negative equivalent

Available options:
        "None"   	 - 	 No cutoff
        "ByLine" 	 - 	 Cutoff relative line center, highest frequency:
F0+cutoff "ByBand" 	 - 	 Absolute frequency, highest frequency: cutoff

For "ByLine", the negative frequency is at F0-cutoff
For "ByBand", the negative frequency is at cutoff minus twice the average band
frequency

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] option - Method of line shape calculations
@param[in] value - Value of cutoff

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesSetCutoff(Workspace& ws, const String& option,
                        const Numeric& value) {
  abs_linesSetCutoff(Var::abs_lines(ws), option, value, Var::verbosity(ws));
}

/*! See *abs_linesSetCutoff* for more options.

This function only acts on matches between the bands and input ID

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] option - Method of line shape calculations
@param[in] value - Value of cutoff
@param[in] ID - ID of one or more bands

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesSetCutoffForMatch(Workspace& ws, const String& option,
                                const Numeric& value,
                                const QuantumIdentifier& ID) {
  abs_linesSetCutoffForMatch(Var::abs_lines(ws), option, value, ID,
                             Var::verbosity(ws));
}

/*! Sets a broadening parameter to empty if it is efficiently empty

This will not save RAM but it will save disk space (reading time),
and computational time by not doing unecessary calculations

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesSetEmptyBroadeningParametersToEmpty(Workspace& ws) {
  abs_linesSetEmptyBroadeningParametersToEmpty(Var::abs_lines(ws),
                                               Var::verbosity(ws));
}

/*! Sets line shape model data parameter in matching lines.

The matching is done so that QI must be in the line identifier

Acceptable parameter(s) are:
        "G0"
        "D0"
        "G2"
        "D2"
        "FVC"
        "ETA"
        "Y"
        "G"
        "DV"

Acceptable coefficient(s) are:
        "X0"
        "X1"
        "X2"
        "X3"

Acceptable species are:
        "AIR" (so long as it is the broadening species list)
        "SELF" (so long as it is the broadening species list)
        Any species in the line broadening species

Throws an error if it cannot find any targets to change

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] QI - Information to match the line.
@param[in] parameter - Name of parameter to be replaced
@param[in] coefficient - Coefficient of the parameter to be changed
@param[in] species - Species of parameter to be changed
@param[in] change - Sets the value found

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesSetLineShapeModelParameterForMatchingLines(
    Workspace& ws, const QuantumIdentifier& QI, const String& parameter,
    const String& coefficient, const String& species, const Numeric& change) {
  abs_linesSetLineShapeModelParameterForMatchingLines(
      Var::abs_lines(ws), QI, parameter, coefficient, species, change,
      Var::verbosity(ws));
}

/*! Sets shape calculations type for all lines.

Available options:
        "DP"   	 - 	 Doppler profile
        "LP"   	 - 	 Lorentz profile
        "VP"   	 - 	 Voigt profile
        "SDVP" 	 - 	 Speed-dependent Voigt profile
        "HTP"  	 - 	 Hartman-Tran profile

See the theory guide for more details.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] option - Method of line shape calculations

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesSetLineShapeType(Workspace& ws, const String& option) {
  abs_linesSetLineShapeType(Var::abs_lines(ws), option, Var::verbosity(ws));
}

/*! See *abs_linesSetLineShapeType* for options

This function only acts on matches between the bands and input ID

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] option - Method of line shape calculations
@param[in] ID - ID of one or more bands

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesSetLineShapeTypeForMatch(Workspace& ws, const String& option,
                                       const QuantumIdentifier& ID) {
  abs_linesSetLineShapeTypeForMatch(Var::abs_lines(ws), option, ID,
                                    Var::verbosity(ws));
}

/*! Sets line mixing limit for all lines.

If value is less than 0, no limit is applied and line mixing is active.
Otherwise, line mixing is inactive if the pressure is below the limit.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] value - Value of limit

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesSetLinemixingLimit(Workspace& ws, const Numeric& value) {
  abs_linesSetLinemixingLimit(Var::abs_lines(ws), value, Var::verbosity(ws));
}

/*! See *abs_linesSetLinemixingLimit* for values

This function only acts on matches between the bands and input ID

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] value - Value of limit
@param[in] ID - ID of one or more bands

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesSetLinemixingLimitForMatch(Workspace& ws, const Numeric& value,
                                         const QuantumIdentifier& ID) {
  abs_linesSetLinemixingLimitForMatch(Var::abs_lines(ws), value, ID,
                                      Var::verbosity(ws));
}

/*! Sets mirroring type for all lines.

Available options:
        "None"    	 - 	 No mirrored line
        "Same"    	 - 	 Mirrored line broadened by line shape
        "Manual"  	 - 	 Manually mirrored line (be careful; allows all
frequencies) "Lorentz" 	 - 	 Mirrored line broadened by Lorentz

Note that mirroring is never applied for DP line shape
Also note that Lorentz profile is approached by most line shapes at high
frequency offset. Also note that Manual settings are potentially dangerous as
other frequency offsets might not work as hoped.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] option - Method of line mirroring

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesSetMirroring(Workspace& ws, const String& option) {
  abs_linesSetMirroring(Var::abs_lines(ws), option, Var::verbosity(ws));
}

/*! See *abs_linesSetMirroring* for options

This function only acts on matches between the bands and input ID

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] option - Method of line mirroring
@param[in] ID - ID of one or more bands

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesSetMirroringForMatch(Workspace& ws, const String& option,
                                   const QuantumIdentifier& ID) {
  abs_linesSetMirroringForMatch(Var::abs_lines(ws), option, ID,
                                Var::verbosity(ws));
}

/*! Sets normalization type for all lines.

Available options:
        "VVH"  	 - 	 Van Vleck and Huber
        "VVW"  	 - 	 Van Vleck and Weisskopf
        "RQ"   	 - 	 Rosenkranz quadratic
        "None" 	 - 	 No extra normalization

See the theory guide for more details.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] option - Method of line normalizations

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesSetNormalization(Workspace& ws, const String& option) {
  abs_linesSetNormalization(Var::abs_lines(ws), option, Var::verbosity(ws));
}

/*! See *abs_linesSetNormalization* for options

This function only acts on matches between the bands and input ID

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] option - Method of line normalizations
@param[in] ID - ID of one or more bands

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesSetNormalizationForMatch(Workspace& ws, const String& option,
                                       const QuantumIdentifier& ID) {
  abs_linesSetNormalizationForMatch(Var::abs_lines(ws), option, ID,
                                    Var::verbosity(ws));
}

/*! Sets population type for all lines.

Available options:
        "LTE"                          	 - 	 Standard distribution by
temperature
        "NLTE-VibrationalTemperatures" 	 - 	 LTE but with vibrational
temperatures
        "NLTE"                         	 - 	 Distribution is given as input

You must have set *nlte_field* and/or its ilk to use the NLTE methods.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] option - Method of line population

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesSetPopulation(Workspace& ws, const String& option) {
  abs_linesSetPopulation(Var::abs_lines(ws), option, Var::verbosity(ws));
}

/*! See *abs_linesSetPopulation* for options

This function only acts on matches between the bands and input ID

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] option - Method of line population
@param[in] ID - ID of one or more bands

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesSetPopulationForMatch(Workspace& ws, const String& option,
                                    const QuantumIdentifier& ID) {
  abs_linesSetPopulationForMatch(Var::abs_lines(ws), option, ID,
                                 Var::verbosity(ws));
}

/*! Sets a quantum number to a new value

This function only acts on matches between the bands and input ID

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] quantum_number - Quantum number key
@param[in] value - Value of quantum number
@param[in] ID - ID of one or more bands

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesSetQuantumNumberForMatch(Workspace& ws,
                                       const String& quantum_number,
                                       const Rational& value,
                                       const QuantumIdentifier& ID) {
  abs_linesSetQuantumNumberForMatch(Var::abs_lines(ws), quantum_number, value,
                                    ID, Var::verbosity(ws));
}

/*! Sets reference temperature for all lines.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] value - Value of T0

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesSetT0(Workspace& ws, const Numeric& value) {
  abs_linesSetT0(Var::abs_lines(ws), value, Var::verbosity(ws));
}

/*! Sets reference temperature

This function only acts on matches between the bands and input ID

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] value - Value of T0
@param[in] ID - ID of one or more bands

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesSetT0ForMatch(Workspace& ws, const Numeric& value,
                            const QuantumIdentifier& ID) {
  abs_linesSetT0ForMatch(Var::abs_lines(ws), value, ID, Var::verbosity(ws));
}

/*! Truncates all global quantum numbers
and then recombine the line list.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesTruncateGlobalQuantumNumbers(Workspace& ws) {
  abs_linesTruncateGlobalQuantumNumbers(Var::abs_lines(ws), Var::verbosity(ws));
}

/*! As *abs_linesWriteSplitXML* but writes an array
per species

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] basename - Path to store the files at

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesWriteSpeciesSplitXML(Workspace& ws, const String& basename) {
  abs_linesWriteSpeciesSplitXML(Var::output_file_format(ws), Var::abs_lines(ws),
                                basename, Var::verbosity(ws));
}

/*! Writes a split catalog, AbsorptionLines by AbsorptionLines.

There will be one unique file generated per AbsorptionLines in *abs_lines*.

The names of these files will be:
        basename+"."+AbsorptionLines.SpeciesName()+"."+to_string(N)+".xml"
where N>=0 and the species name is something line "H2O".

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] basename - Path to store the files at

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_linesWriteSplitXML(Workspace& ws, const String& basename) {
  abs_linesWriteSplitXML(Var::output_file_format(ws), Var::abs_lines(ws),
                         basename, Var::verbosity(ws));
}

/*! See *abs_linesChangeBaseParameterForMatchingLevel*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] QI - Information to match the level.
@param[in] parameter_name - Name of parameter to be replaced
@param[in] change - Value with which to change matching level's value
@param[in] relative - Flag for relative change (0 is absolute change) (default:
0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesChangeBaseParameterForMatchingLevel(
    Workspace& ws, const QuantumIdentifier& QI, const String& parameter_name,
    const Numeric& change, const Index& relative = 0) {
  abs_lines_per_speciesChangeBaseParameterForMatchingLevel(
      Var::abs_lines_per_species(ws), QI, parameter_name, change, relative,
      Var::verbosity(ws));
}

/*! See *abs_linesChangeBaseParameterForMatchingLevel*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] QI - Information to match the level.
@param[in] parameter_name - Name of parameter to be replaced
@param[in] change - Value with which to change matching level's value
@param[in] relative - Flag for relative change (0 is absolute change) (default:
0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesChangeBaseParameterForMatchingLevels(
    Workspace& ws, const ArrayOfQuantumIdentifier& QI,
    const String& parameter_name, const Vector& change,
    const Index& relative = 0) {
  abs_lines_per_speciesChangeBaseParameterForMatchingLevels(
      Var::abs_lines_per_species(ws), QI, parameter_name, change, relative,
      Var::verbosity(ws));
}

/*! See *abs_linesChangeBaseParameterForMatchingLines*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] QI - Information to match the line.
@param[in] parameter_name - Name of parameter to be replaced
@param[in] change - Value with which to change matching line's value
@param[in] relative - Flag for relative change (0 is absolute change) (default:
0)
@param[in] loose_matching - Flag for loose match (0 means only complete matches)
(default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesChangeBaseParameterForMatchingLines(
    Workspace& ws, const QuantumIdentifier& QI, const String& parameter_name,
    const Numeric& change, const Index& relative = 0,
    const Index& loose_matching = 0) {
  abs_lines_per_speciesChangeBaseParameterForMatchingLines(
      Var::abs_lines_per_species(ws), QI, parameter_name, change, relative,
      loose_matching, Var::verbosity(ws));
}

/*! See *abs_linesChangeBaseParameterForMatchingLines* but for single species

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] QI - Information to match the line.
@param[in] parameter_name - Name of parameter to be replaced
@param[in] change - Value with which to change matching line's value
@param[in] relative - Flag for relative change (0 is absolute change) (default:
0)
@param[in] loose_matching - Flag for loose match (0 means only complete matches)
(default: 0)
@param[in] species_tag - The species tag from *abs_species* to change

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesChangeBaseParameterForSpecies(
    Workspace& ws, const QuantumIdentifier& QI, const String& parameter_name,
    const Numeric& change, const String& species_tag, const Index& relative = 0,
    const Index& loose_matching = 0) {
  abs_lines_per_speciesChangeBaseParameterForSpecies(
      Var::abs_lines_per_species(ws), Var::abs_species(ws), QI, parameter_name,
      change, relative, loose_matching, species_tag, Var::verbosity(ws));
}

/*! See *abs_linesChangeLineShapeModelParameterForMatchingLines*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] QI - Information to match the line.
@param[in] parameter - Name of parameter to be replaced
@param[in] coefficient - Coefficient of the parameter to be changed
@param[in] species - Species of parameter to be changed
@param[in] change - Change in the value found
@param[in] relative - Flag for relative change (0 is absolute change) (default:
0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesChangeLineShapeModelParameterForMatchingLines(
    Workspace& ws, const QuantumIdentifier& QI, const String& parameter,
    const String& coefficient, const String& species, const Numeric& change,
    const Index& relative = 0) {
  abs_lines_per_speciesChangeLineShapeModelParameterForMatchingLines(
      Var::abs_lines_per_species(ws), QI, parameter, coefficient, species,
      change, relative, Var::verbosity(ws));
}

/*! See *abs_linesChangeLineShapeModelParameterForMatchingLines*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] QI - Information to match the line.
@param[in] parameter - Name of parameter to be replaced
@param[in] coefficient - Coefficient of the parameter to be changed
@param[in] species - Species of parameter to be changed
@param[in] change - Change in the value found
@param[in] relative - Flag for relative change (0 is absolute change) (default:
0)
@param[in] species_tag - The species tag from *abs_species* to change

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesChangeLineShapeModelParameterForSpecies(
    Workspace& ws, const QuantumIdentifier& QI, const String& parameter,
    const String& coefficient, const String& species, const Numeric& change,
    const String& species_tag, const Index& relative = 0) {
  abs_lines_per_speciesChangeLineShapeModelParameterForSpecies(
      Var::abs_lines_per_species(ws), Var::abs_species(ws), QI, parameter,
      coefficient, species, change, relative, species_tag, Var::verbosity(ws));
}

/*! See *abs_linesCompact*

@author Stefan Buehler
@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesCompact(Workspace& ws) {
  abs_lines_per_speciesCompact(Var::abs_lines_per_species(ws), Var::f_grid(ws),
                               Var::verbosity(ws));
}

/*! Split lines up into the different species.

The order of the splitting will match the outer layer of *abs_species*
There will be no respect for the internal layer of *abs_species*

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesCreateFromLines(Workspace& ws) {
  abs_lines_per_speciesCreateFromLines(Var::abs_lines_per_species(ws),
                                       Var::abs_lines(ws), Var::abs_species(ws),
                                       Var::verbosity(ws));
}

/*! See *abs_lines_per_speciesReadSplitCatalog* but expects
a single file per species of *ArrayOfAbsorptionLines*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] basename - The path to the split catalog files
@param[in] robust - Flag to continue in case nothing is found [0 throws, 1
continues] (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesReadSpeciesSplitCatalog(Workspace& ws,
                                                  const String& basename,
                                                  const Index& robust = 0) {
  abs_lines_per_speciesReadSpeciesSplitCatalog(Var::abs_lines_per_species(ws),
                                               Var::abs_species(ws), basename,
                                               robust, Var::verbosity(ws));
}

/*! Reads *abs_lines_per_species* split by
*abs_linesWriteSplitXML* or *abs_lines_per_speciesWriteSplitXML*

Note that this will sort the isotopologue

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] basename - The path to the split catalog files

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesReadSplitCatalog(Workspace& ws,
                                           const String& basename) {
  abs_lines_per_speciesReadSplitCatalog(Var::abs_lines_per_species(ws),
                                        Var::abs_species(ws), basename,
                                        Var::verbosity(ws));
}

/*! See *abs_linesSetBaseParameterForMatchingLevel*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] QI - Information to match the level.
@param[in] parameter_name - Name of parameter to be replaced
@param[in] change - Value with which to set matching level's value

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesSetBaseParameterForMatchingLevel(
    Workspace& ws, const QuantumIdentifier& QI, const String& parameter_name,
    const Numeric& change) {
  abs_lines_per_speciesSetBaseParameterForMatchingLevel(
      Var::abs_lines_per_species(ws), QI, parameter_name, change,
      Var::verbosity(ws));
}

/*! See *abs_linesSetBaseParameterForMatchingLevel*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] QI - Information to match the level.
@param[in] parameter_name - Name of parameter to be replaced
@param[in] change - Value with which to set matching level's value

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesSetBaseParameterForMatchingLevels(
    Workspace& ws, const ArrayOfQuantumIdentifier& QI,
    const String& parameter_name, const Vector& change) {
  abs_lines_per_speciesSetBaseParameterForMatchingLevels(
      Var::abs_lines_per_species(ws), QI, parameter_name, change,
      Var::verbosity(ws));
}

/*! See *abs_linesSetBaseParameterForMatchingLines*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] QI - Information to match the line.
@param[in] parameter_name - Name of parameter to be replaced
@param[in] change - Value with which to change matching line's value
@param[in] loose_matching - Flag for loose match (0 means only complete matches)
(default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesSetBaseParameterForMatchingLines(
    Workspace& ws, const QuantumIdentifier& QI, const String& parameter_name,
    const Numeric& change, const Index& loose_matching = 0) {
  abs_lines_per_speciesSetBaseParameterForMatchingLines(
      Var::abs_lines_per_species(ws), QI, parameter_name, change,
      loose_matching, Var::verbosity(ws));
}

/*! See *abs_linesSetBaseParameterForMatchingLines* but for single species

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] QI - Information to match the line.
@param[in] parameter_name - Name of parameter to be replaced
@param[in] change - Value with which to change matching line's value
@param[in] loose_matching - Flag for loose match (0 means only complete matches)
(default: 0)
@param[in] species_tag - The species tag from *abs_species* to change

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesSetBaseParameterForSpecies(
    Workspace& ws, const QuantumIdentifier& QI, const String& parameter_name,
    const Numeric& change, const String& species_tag,
    const Index& loose_matching = 0) {
  abs_lines_per_speciesSetBaseParameterForSpecies(
      Var::abs_lines_per_species(ws), Var::abs_species(ws), QI, parameter_name,
      change, loose_matching, species_tag, Var::verbosity(ws));
}

/*! See *abs_linesSetCutoff*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] option - Method of line shape calculations
@param[in] value - Value of cutoff

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesSetCutoff(Workspace& ws, const String& option,
                                    const Numeric& value) {
  abs_lines_per_speciesSetCutoff(Var::abs_lines_per_species(ws), option, value,
                                 Var::verbosity(ws));
}

/*! See *abs_linesSetCutoff* for more options.

This function only acts on matches between the bands and input ID

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] option - Method of line shape calculations
@param[in] value - Value of cutoff
@param[in] ID - ID of one or more bands

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesSetCutoffForMatch(Workspace& ws, const String& option,
                                            const Numeric& value,
                                            const QuantumIdentifier& ID) {
  abs_lines_per_speciesSetCutoffForMatch(Var::abs_lines_per_species(ws), option,
                                         value, ID, Var::verbosity(ws));
}

/*! See *abs_linesSetCutoff* but for single species

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] option - Method of line shape calculations
@param[in] value - Value of cutoff
@param[in] species_tag - The species tag from *abs_species* to change

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesSetCutoffForSpecies(Workspace& ws,
                                              const String& option,
                                              const Numeric& value,
                                              const String& species_tag) {
  abs_lines_per_speciesSetCutoffForSpecies(Var::abs_lines_per_species(ws),
                                           Var::abs_species(ws), option, value,
                                           species_tag, Var::verbosity(ws));
}

/*! Empties *abs_lines_per_species* at the correct size.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesSetEmpty(Workspace& ws) {
  abs_lines_per_speciesSetEmpty(Var::abs_lines_per_species(ws),
                                Var::abs_species(ws), Var::verbosity(ws));
}

/*! See *abs_linesSetLineShapeModelParameterForMatchingLines*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] QI - Information to match the line.
@param[in] parameter - Name of parameter to be replaced
@param[in] coefficient - Coefficient of the parameter to be changed
@param[in] species - Species of parameter to be changed
@param[in] change - Sets the value found

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesSetLineShapeModelParameterForMatchingLines(
    Workspace& ws, const QuantumIdentifier& QI, const String& parameter,
    const String& coefficient, const String& species, const Numeric& change) {
  abs_lines_per_speciesSetLineShapeModelParameterForMatchingLines(
      Var::abs_lines_per_species(ws), QI, parameter, coefficient, species,
      change, Var::verbosity(ws));
}

/*! See *abs_linesSetLineShapeModelParameterForMatchingLines*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] QI - Information to match the line.
@param[in] parameter - Name of parameter to be replaced
@param[in] coefficient - Coefficient of the parameter to be changed
@param[in] species - Species of parameter to be changed
@param[in] change - Sets the value found
@param[in] species_tag - The species tag from *abs_species* to change

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesSetLineShapeModelParameterForSpecies(
    Workspace& ws, const QuantumIdentifier& QI, const String& parameter,
    const String& coefficient, const String& species, const Numeric& change,
    const String& species_tag) {
  abs_lines_per_speciesSetLineShapeModelParameterForSpecies(
      Var::abs_lines_per_species(ws), Var::abs_species(ws), QI, parameter,
      coefficient, species, change, species_tag, Var::verbosity(ws));
}

/*! See *abs_linesSetLineShapeType*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] option - Method of line shape calculations

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesSetLineShapeType(Workspace& ws,
                                           const String& option) {
  abs_lines_per_speciesSetLineShapeType(Var::abs_lines_per_species(ws), option,
                                        Var::verbosity(ws));
}

/*! See *abs_linesSetLineShapeType* for options

This function only acts on matches between the bands and input ID

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] option - Method of line shape calculations
@param[in] ID - ID of one or more bands

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesSetLineShapeTypeForMatch(
    Workspace& ws, const String& option, const QuantumIdentifier& ID) {
  abs_lines_per_speciesSetLineShapeTypeForMatch(Var::abs_lines_per_species(ws),
                                                option, ID, Var::verbosity(ws));
}

/*! See *abs_linesSetLineShapeType* but for single species

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] option - Method of line shape calculations
@param[in] species_tag - The species tag from *abs_species* to change

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesSetLineShapeTypeForSpecies(
    Workspace& ws, const String& option, const String& species_tag) {
  abs_lines_per_speciesSetLineShapeTypeForSpecies(
      Var::abs_lines_per_species(ws), Var::abs_species(ws), option, species_tag,
      Var::verbosity(ws));
}

/*! See *abs_linesSetLinemixingLimit*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] value - Value of limit

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesSetLinemixingLimit(Workspace& ws,
                                             const Numeric& value) {
  abs_lines_per_speciesSetLinemixingLimit(Var::abs_lines_per_species(ws), value,
                                          Var::verbosity(ws));
}

/*! See *abs_linesSetLinemixingLimit* for values

This function only acts on matches between the bands and input ID

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] value - Value of limit
@param[in] ID - ID of one or more bands

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesSetLinemixingLimitForMatch(
    Workspace& ws, const Numeric& value, const QuantumIdentifier& ID) {
  abs_lines_per_speciesSetLinemixingLimitForMatch(
      Var::abs_lines_per_species(ws), value, ID, Var::verbosity(ws));
}

/*! See *abs_linesSetLinemixingLimit* but for single species

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] value - Value of limit
@param[in] species_tag - The species tag from *abs_species* to change

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesSetLinemixingLimitForSpecies(
    Workspace& ws, const Numeric& value, const String& species_tag) {
  abs_lines_per_speciesSetLinemixingLimitForSpecies(
      Var::abs_lines_per_species(ws), Var::abs_species(ws), value, species_tag,
      Var::verbosity(ws));
}

/*! See *abs_linesSetMirroring*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] option - Method of line mirroring

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesSetMirroring(Workspace& ws, const String& option) {
  abs_lines_per_speciesSetMirroring(Var::abs_lines_per_species(ws), option,
                                    Var::verbosity(ws));
}

/*! See *abs_linesSetMirroring* for options

This function only acts on matches between the bands and input ID

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] option - Method of line mirroring
@param[in] ID - ID of one or more bands

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesSetMirroringForMatch(Workspace& ws,
                                               const String& option,
                                               const QuantumIdentifier& ID) {
  abs_lines_per_speciesSetMirroringForMatch(Var::abs_lines_per_species(ws),
                                            option, ID, Var::verbosity(ws));
}

/*! See *abs_linesSetMirroring* but for single species

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] option - Method of line mirroring
@param[in] species_tag - The species tag from *abs_species* to change

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesSetMirroringForSpecies(Workspace& ws,
                                                 const String& option,
                                                 const String& species_tag) {
  abs_lines_per_speciesSetMirroringForSpecies(Var::abs_lines_per_species(ws),
                                              Var::abs_species(ws), option,
                                              species_tag, Var::verbosity(ws));
}

/*! See *abs_linesSetNormalization*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] option - Method of line normalizations

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesSetNormalization(Workspace& ws,
                                           const String& option) {
  abs_lines_per_speciesSetNormalization(Var::abs_lines_per_species(ws), option,
                                        Var::verbosity(ws));
}

/*! See *abs_linesSetNormalization* for options

This function only acts on matches between the bands and input ID

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] option - Method of line normalizations
@param[in] ID - ID of one or more bands

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesSetNormalizationForMatch(
    Workspace& ws, const String& option, const QuantumIdentifier& ID) {
  abs_lines_per_speciesSetNormalizationForMatch(Var::abs_lines_per_species(ws),
                                                option, ID, Var::verbosity(ws));
}

/*! See *abs_linesSetNormalization* but for single species

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] option - Method of line normalizations
@param[in] species_tag - The species tag from *abs_species* to change

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesSetNormalizationForSpecies(
    Workspace& ws, const String& option, const String& species_tag) {
  abs_lines_per_speciesSetNormalizationForSpecies(
      Var::abs_lines_per_species(ws), Var::abs_species(ws), option, species_tag,
      Var::verbosity(ws));
}

/*! See *abs_linesSetPopulation*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] option - Method of line population

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesSetPopulation(Workspace& ws, const String& option) {
  abs_lines_per_speciesSetPopulation(Var::abs_lines_per_species(ws), option,
                                     Var::verbosity(ws));
}

/*! See *abs_linesSetPopulation* for options

This function only acts on matches between the bands and input ID

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] option - Method of line population
@param[in] ID - ID of one or more bands

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesSetPopulationForMatch(Workspace& ws,
                                                const String& option,
                                                const QuantumIdentifier& ID) {
  abs_lines_per_speciesSetPopulationForMatch(Var::abs_lines_per_species(ws),
                                             option, ID, Var::verbosity(ws));
}

/*! See *abs_linesSetPopulation* but for single species

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] option - Method of line population
@param[in] species_tag - The species tag from *abs_species* to change

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesSetPopulationForSpecies(Workspace& ws,
                                                  const String& option,
                                                  const String& species_tag) {
  abs_lines_per_speciesSetPopulationForSpecies(Var::abs_lines_per_species(ws),
                                               Var::abs_species(ws), option,
                                               species_tag, Var::verbosity(ws));
}

/*! See *abs_linesSetQuantumNumberForMatch*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] quantum_number - Quantum number key
@param[in] value - Value of quantum number
@param[in] ID - ID of one or more bands

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesSetQuantumNumberForMatch(
    Workspace& ws, const String& quantum_number, const Rational& value,
    const QuantumIdentifier& ID) {
  abs_lines_per_speciesSetQuantumNumberForMatch(Var::abs_lines_per_species(ws),
                                                quantum_number, value, ID,
                                                Var::verbosity(ws));
}

/*! See *abs_linesSetT0*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] value - Value of T0

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesSetT0(Workspace& ws, const Numeric& value) {
  abs_lines_per_speciesSetT0(Var::abs_lines_per_species(ws), value,
                             Var::verbosity(ws));
}

/*! Sets reference temperature

This function only acts on matches between the bands and input ID

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] value - Value of T0
@param[in] ID - ID of one or more bands

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesSetT0ForMatch(Workspace& ws, const Numeric& value,
                                        const QuantumIdentifier& ID) {
  abs_lines_per_speciesSetT0ForMatch(Var::abs_lines_per_species(ws), value, ID,
                                     Var::verbosity(ws));
}

/*! See *abs_linesSetT0* but for single species

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] value - Value of T0
@param[in] species_tag - The species tag from *abs_species* to change

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesSetT0ForSpecies(Workspace& ws, const Numeric& value,
                                          const String& species_tag) {
  abs_lines_per_speciesSetT0ForSpecies(Var::abs_lines_per_species(ws),
                                       Var::abs_species(ws), value, species_tag,
                                       Var::verbosity(ws));
}

/*! See *abs_linesWriteSpeciesSplitXML*

In addition, the structure of the files generated will not care about
generating identifiers for the order in *abs_species*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] basename - Path to store the files at

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesWriteSpeciesSplitXML(Workspace& ws,
                                               const String& basename) {
  abs_lines_per_speciesWriteSpeciesSplitXML(Var::output_file_format(ws),
                                            Var::abs_lines_per_species(ws),
                                            basename, Var::verbosity(ws));
}

/*! See *abs_linesWriteSplitXML*

In addition, the structure of the files generated will not care about
generating identifiers for the order in *abs_species*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] basename - Path to store the files at

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lines_per_speciesWriteSplitXML(Workspace& ws, const String& basename) {
  abs_lines_per_speciesWriteSplitXML(Var::output_file_format(ws),
                                     Var::abs_lines_per_species(ws), basename,
                                     Var::verbosity(ws));
}

/*! Adapts a gas absorption lookup table to the current calculation.

The lookup table can contain more species and more frequencies than
are needed for the current calculation. This method cuts down the
table in memory, so that it contains just what is needed. Also, the
species in the table are brought in the same order as the species in
the current calculation.

Of course, the method also performs quite a lot of checks on the
table. If something is not ok, a runtime error is thrown.

The method sets a flag *abs_lookup_is_adapted* to indicate that the
table has been checked and that it is ok. Never set this by hand,
always use this method to set it!

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lookupAdapt(Workspace& ws) {
  abs_lookupAdapt(Var::abs_lookup(ws), Var::abs_lookup_is_adapted(ws),
                  Var::abs_species(ws), Var::f_grid(ws), Var::verbosity(ws));
}

/*! Creates a gas absorption lookup table.

The lookup table stores absorption cross-sections as a function of
pressure. Additionally, absorption can be stored as a function of
temperature for temperature perturbations from a reference
profile.

Additionally, absorption can be stored as a function of water vapor
VMR perturbations from a reference profile. The variable *abs_nls*
specifies, for which species water vapor perturbations should be
generated.

Note, that the absorbing gas can be any gas, but the perturbing gas is
always H2O.

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lookupCalc(Workspace& ws) {
  abs_lookupCalc(ws, Var::abs_lookup(ws), Var::abs_lookup_is_adapted(ws),
                 Var::abs_species(ws), Var::abs_nls(ws), Var::f_grid(ws),
                 Var::abs_p(ws), Var::abs_vmrs(ws), Var::abs_t(ws),
                 Var::abs_t_pert(ws), Var::abs_nls_pert(ws),
                 Var::abs_xsec_agenda(ws), Var::verbosity(ws));
}

/*! Creates an empty gas absorption lookup table.

This is mainly there to help developers. For example, you can write
the empty table to an XML file, to see the file format.

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lookupInit(Workspace& ws) {
  abs_lookupInit(Var::abs_lookup(ws), Var::verbosity(ws));
}

/*! Set up input parameters for abs_lookupCalc.

More information can be found in the documentation for method
*abs_lookupSetupBatch*

Max and min values of H2O and temperature are adjusted to allow for
numerical perturbations in Jacobian calculation.

The input variables *abs_nls_interp_order* and *abs_t_interp_order*
are used to make sure that there are enough points in *abs_nls_pert*
and *abs_t_pert* for the chosen interpolation order.

Note: For homogeneous 1D cases, it can be advantageous to calculate
*abs_lookup* from the 1D atmosphere, and to expand the atmosphere
to 3D only after that. This particularly if nonlinear species
(i.e., H2O) are involved.
See also:
   *abs_lookupSetupBatch*

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace
@param[in] p_step - Maximum step in log10(p[Pa]). If the pressure grid is
coarser than this, additional points are added until each log step is smaller
than this. (default: 0.05)
@param[in] t_step - The temperature variation grid step in Kelvin, for a 2D or
3D atmosphere. For a 1D atmosphere this parameter is not used. (default: 100)
@param[in] h2o_step - The H2O variation grid step [fractional], if H2O
variations are done (which is determined automatically, based on abs_species and
the atmospheric dimension). For a 1D atmosphere this parameter is not used.
(default: 100)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lookupSetup(Workspace& ws, const Numeric& p_step = 0.05,
                     const Numeric& t_step = 100,
                     const Numeric& h2o_step = 100) {
  abs_lookupSetup(Var::abs_p(ws), Var::abs_t(ws), Var::abs_t_pert(ws),
                  Var::abs_vmrs(ws), Var::abs_nls(ws), Var::abs_nls_pert(ws),
                  Var::atmosphere_dim(ws), Var::p_grid(ws), Var::t_field(ws),
                  Var::vmr_field(ws), Var::atmfields_checked(ws),
                  Var::abs_species(ws), Var::abs_p_interp_order(ws),
                  Var::abs_t_interp_order(ws), Var::abs_nls_interp_order(ws),
                  p_step, t_step, h2o_step, Var::verbosity(ws));
}

/*! Set up input parameters for abs_lookupCalc for batch calculations.

This method performs a similar task as *abs_lookupSetup*, with the
difference that the lookup table setup is not for a single
atmospheric state, but for a whole batch of them, stored in
*batch_atm_fields_compact*.

The method checks *abs_species* to decide which species require
nonlinear treatment in the lookup table.

The method also checks which range of pressures, temperatures, and
VMRs occurs, and sets *abs_p*, *abs_t*, *abs_t_pert*, and *abs_vmrs*
accordingly.

If nonlinear species are present, *abs_nls* and *abs_nls_pert* are also
generated.

Max and min values of H2O and temperature are adjusted to allow for
numerical perturbations in Jacobian calculation.

The input variables *abs_nls_interp_order* and *abs_t_interp_order*
are used to make sure that there are enough points in *abs_nls_pert*
and *abs_t_pert* for the chosen interpolation order.

The method checks each given field using *atmfields_checkedCalc*.
If a field does not pass the check, a run-time error is thrown.
To prevent this, the parameter *robust* can be set to one: Invalid
atmospheres are skipped, but the run continues. This matches the
robust behaviour of *ybatchCalc*.

See also:
   *abs_lookupSetup*

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace
@param[in] p_step - Grid step in log10(p[Pa]) (base 10 logarithm). (default:
0.05)
@param[in] t_step - The temperature variation grid step in Kelvin. The true step
can become finer than this, if required by the interpolation order. (default:
20)
@param[in] h2o_step - The H2O variation grid step [fractional], if H2O
variations are done (which is determined automatically, based on abs_species and
the atmospheric dimension). As for T, the true step can turn out finer if
required by the interpolation order. (default: 100)
@param[in] extremes - You can give here explicit extreme values to add to
abs_t_pert and abs_nls_pert. The order is [t_pert_min, t_pert_max, nls_pert_min,
nls_pert_max]. (default: {})
@param[in] robust - A flag with value 1 or 0. If set to one, the batch
setup will continue, even if individual fields are invalid.
This is consistent with the behaviour of *ybatchCalc*. (default: 0)
@param[in] check_gridnames - A flag with value 1 or 0. If set to one, the
gridnames of every *atm_fields_compact* are checked. (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lookupSetupBatch(Workspace& ws, const Numeric& p_step = 0.05,
                          const Numeric& t_step = 20,
                          const Numeric& h2o_step = 100,
                          const Vector& extremes = {}, const Index& robust = 0,
                          const Index& check_gridnames = 0) {
  abs_lookupSetupBatch(
      Var::abs_p(ws), Var::abs_t(ws), Var::abs_t_pert(ws), Var::abs_vmrs(ws),
      Var::abs_nls(ws), Var::abs_nls_pert(ws), Var::abs_species(ws),
      Var::batch_atm_fields_compact(ws), Var::abs_p_interp_order(ws),
      Var::abs_t_interp_order(ws), Var::abs_nls_interp_order(ws),
      Var::atmosphere_dim(ws), p_step, t_step, h2o_step, extremes, robust,
      check_gridnames, Var::verbosity(ws));
}

/*! Set up input parameters for abs_lookupCalc for a wide range of
atmospheric conditions.

This method can be used to set up parameters for a lookup table that
really covers all reasonable atmospheric conditions.

Reference profiles of T and H2O will be constant, so that the
different dimensions in the lookup table are actually "orthogonal",
unlike the traditional case where we have pressure dependent reference
profiles. This makes the table numerically somewhat more robust then
the traditional ones, and it makes it straightforward to calculate the
accuracy for the different interpolations with abs_lookupTestAccuracy.

You can give min an max values for the atmospheric conditions. The
default values are chosen such that they cover the value range over
the complete Chevallier91L data set, and a bit more. The statistics
of the Chevallier91L data are:

min(p)   / max(p)   [Pa]:  1 / 104960
min(T)   / max(T)   [K]:   158.21 / 320.39
min(H2O) / max(H2O) [VMR]: -5.52e-07 / 0.049

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace
@param[in] p_min - Pressure grid minimum [Pa]. (default: 0.5)
@param[in] p_max - Pressure grid maximum [Pa]. (default: 110000)
@param[in] p_step - Pressure grid step in log10(p[Pa]) (base 10 logarithm).
(default: 0.05)
@param[in] t_min - Temperature grid minimum [K]. (default: 100)
@param[in] t_max - Temperature grid maximum [K]. (default: 400)
@param[in] h2o_min - Humidity grid minimum [fractional]. (default: 0)
@param[in] h2o_max - Humidity grid maximum [fractional]. (default: 0.05)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lookupSetupWide(Workspace& ws, const Numeric& p_min = 0.5,
                         const Numeric& p_max = 110000,
                         const Numeric& p_step = 0.05,
                         const Numeric& t_min = 100, const Numeric& t_max = 400,
                         const Numeric& h2o_min = 0,
                         const Numeric& h2o_max = 0.05) {
  abs_lookupSetupWide(Var::abs_p(ws), Var::abs_t(ws), Var::abs_t_pert(ws),
                      Var::abs_vmrs(ws), Var::abs_nls(ws),
                      Var::abs_nls_pert(ws), Var::abs_species(ws),
                      Var::abs_p_interp_order(ws), Var::abs_t_interp_order(ws),
                      Var::abs_nls_interp_order(ws), p_min, p_max, p_step,
                      t_min, t_max, h2o_min, h2o_max, Var::verbosity(ws));
}

/*! Test accuracy of absorption lookup table with Monte Carlo Algorithm.

Explicitly compare absorption from the lookup table with line-by-line
calculations for random conditions.

The quantities returned are the mean value and standard deviation of
the absolute value of the relative error in percent.
The relative error itself is computed for a large number of cases
(pressure, temperature, and H2O VMR combinations). In the frequency
dimension the maximum value is taken for each case.

Produces no workspace output, only output to the output streams.

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lookupTestAccMC(Workspace& ws) {
  abs_lookupTestAccMC(ws, Var::abs_lookup(ws), Var::abs_lookup_is_adapted(ws),
                      Var::abs_p_interp_order(ws), Var::abs_t_interp_order(ws),
                      Var::abs_nls_interp_order(ws), Var::mc_seed(ws),
                      Var::abs_xsec_agenda(ws), Var::verbosity(ws));
}

/*! Test accuracy of absorption lookup table.

Explicitly compare absorption from the lookup table with line-by-line
calculations for strategically selected conditions (in-between the
lookup table grid points).

For error units see *abs_lookupTestAccMC*

Produces no workspace output, only output to the output streams.

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_lookupTestAccuracy(Workspace& ws) {
  abs_lookupTestAccuracy(
      ws, Var::abs_lookup(ws), Var::abs_lookup_is_adapted(ws),
      Var::abs_p_interp_order(ws), Var::abs_t_interp_order(ws),
      Var::abs_nls_interp_order(ws), Var::abs_xsec_agenda(ws),
      Var::verbosity(ws));
}

/*! Sets NLTE values manually

Touch

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] data - Vibrational data [nlevels, np]

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_nlteFromRaw(Workspace& ws, const Matrix& data) {
  abs_nlteFromRaw(Var::abs_nlte(ws), Var::nlte_level_identifiers(ws),
                  Var::nlte_vibrational_energies(ws), data, Var::verbosity(ws));
}

/*! Adds species tag groups to the list of absorption species.

This WSM is similar to *abs_speciesSet*, the only difference is that
this method appends species to an existing list of absorption species instead
of creating the whole list.

See *abs_speciesSet* for details on how tags are defined and examples of
how to input them in the control file.

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace
@param[in] species - Specify one String for each tag group that you want to
add. Inside the String, separate the tags by commas
(plus optional blanks).


Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_speciesAdd(Workspace& ws, const ArrayOfString& species) {
  abs_speciesAdd(Var::abs_species(ws), Var::propmat_clearsky_agenda_checked(ws),
                 Var::abs_xsec_agenda_checked(ws), species, Var::verbosity(ws));
}

/*! Adds a species tag group to the list of absorption species and
jacobian quantities.

The method is basically a combined call of *abs_speciesAdd* and
*jacobianAddAbsSpecies*. In this way it is not needed to specify a
tag group in two different places.

Arguments exactly as for *jacobianAddAbsSpecies*. Note that this
method only handles a single tag group, in contrast to
*abs_speciesAdd*.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] gin1 - Pressure retrieval grid.
@param[in] gin2 - Latitude retrieval grid.
@param[in] gin3 - Longitude retreival grid.
@param[in] species - The species tag of the retrieval quantity.
@param[in] unit - Retrieval unit. See above. (default: "vmr")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_speciesAdd2(Workspace& ws, const Vector& gin1, const Vector& gin2,
                     const Vector& gin3, const String& species,
                     const String& unit = "vmr") {
  abs_speciesAdd2(ws, Var::abs_species(ws), Var::jacobian_quantities(ws),
                  Var::jacobian_agenda(ws),
                  Var::propmat_clearsky_agenda_checked(ws),
                  Var::abs_xsec_agenda_checked(ws), Var::atmosphere_dim(ws),
                  Var::p_grid(ws), Var::lat_grid(ws), Var::lon_grid(ws), gin1,
                  gin2, gin3, species, unit, Var::verbosity(ws));
}

/*! Sets *abs_species*[i][0] to all species in ARTS

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_speciesDefineAll(Workspace& ws) {
  abs_speciesDefineAll(Var::abs_species(ws),
                       Var::propmat_clearsky_agenda_checked(ws),
                       Var::abs_xsec_agenda_checked(ws), Var::verbosity(ws));
}

/*! Define one tag group for each species known to ARTS and included in an
atmospheric scenario.

You can use this as an alternative to *abs_speciesSet* if you want to make an
absorption calculation that is as complete as possible. The method
goes through all defined species and tries to open the VMR file. If
this works the tag is included, otherwise it is skipped.

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace
@param[in] basename - The name and path of a particular atmospheric scenario.
For example: /pool/lookup2/arts-data/atmosphere/fascod/tropical

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_speciesDefineAllInScenario(Workspace& ws, const String& basename) {
  abs_speciesDefineAllInScenario(
      Var::abs_species(ws), Var::propmat_clearsky_agenda_checked(ws),
      Var::abs_xsec_agenda_checked(ws), basename, Var::verbosity(ws));
}

/*! Sets  *abs_species* to be empty.

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_speciesInit(Workspace& ws) {
  abs_speciesInit(Var::abs_species(ws), Var::verbosity(ws));
}

/*! Set up a list of absorption species tag groups.

Workspace variables like *abs_species* contain several tag
groups. Each tag group contains one or more tags. This method converts
descriptions of tag groups given in the keyword to the ARTS internal
representation (an *ArrayOfArrayOfSpeciesTag*). A tag group selects
spectral features which belong to the same species.

A tag is defined in terms of the name of the species, isotopologue, and a
range of frequencies. Species are named after the standard chemical
names, e.g., "O3". Isotopologues are given by the last digit of the atomic
weight, i.g., "O3-668" for the asymmetric ozone molecule including an
oxygen 18 atom. Groups of transitions are specified by giving a lower
and upper limit of a frequency range, e.g., "O3-666-500e9-501e9".

To turn on Zeeman calculation for a species, "-Z" may be appended
to its name: "O2-Z" or "O2-Z-66"

To turn on line mixing calculation for a species, "-LM" may be appended
to its name (or after the Zeeman tag): "O2-LM" or "O2-Z-LM-66"

The symbol "*" acts as a wild card. Furthermore, frequency range or
frequency range and isotopologue may be omitted.

Finally, instead of the isotopologue the special letter "nl" may be given,
e.g., "H2O-nl". This means that no absorption at all is associated
with this tag. (It is not quite clear if this feature is useful for
anything right now.)

Example:

   species = [ "O3-666-500e9-501e9, O3-686",
               "O3",
               "H2O-PWR98" ]

   The first tag group selects all O3-666 lines between 500 and
   501 GHz plus all O3-686 lines.

   The second tag group selects all remaining O3 transitions.

   The third tag group selects H2O, with one of the complete
   absorption models (Rosenkranz 98). No spectrocopic line catalogue
   data will be used for that third tag group.

   Note that order of tag groups in the species list matters. In our
   example, changing the order of the first two tag group will give
   different results: as "O3" already selects all O3 transitions,
   no lines will remain to be selected by the
   "O3-666-500e9-501e9, O3-686" tag.

For CIA species the tag consists of the two involved species and
a dataset index. CIA species can be defined for multiple regions
The dataset index determines which region to use from the corresponding
CIARecord in *abs_cia_data*.

Example

species = [ "N2-CIA-N2-0, N2-CIA-N2-1" ]

For Hitran cross section species the tag consists of the species and
the tagtype HXSEC, e.g. CFC11-HXSEC. The data for the species must be
available in the *hitran_xsec_data* variable.
*abs_xsec_agenda_checked* and *propmat_clearsky_agenda_checked*
are set to be false.

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace
@param[in] species - Specify one String for each tag group that you want to
create. Inside the String, separate the tags by commas
(plus optional blanks).


Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_speciesSet(Workspace& ws, const ArrayOfString& species) {
  abs_speciesSet(Var::abs_species(ws), Var::abs_xsec_agenda_checked(ws),
                 Var::propmat_clearsky_agenda_checked(ws), species,
                 Var::verbosity(ws));
}

/*! Add gas absorption to first element of absorption vector.

The task of this method is to sum up the gas absorption of the
different gas species and add the result to the first element of the
absorption vector.

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_vecAddGas(Workspace& ws) {
  abs_vecAddGas(Var::abs_vec(ws), Var::propmat_clearsky(ws),
                Var::verbosity(ws));
}

/*! Checks if the *abs_xsec_agenda* contains all necessary
methods to calculate all the species in *abs_species*.

This method should be called just before the *abs_xsec_agenda*
is used, e.g. *abs_lookupCalc*, *ybatchCalc*, *yCalc*

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_xsec_agenda_checkedCalc(Workspace& ws) {
  abs_xsec_agenda_checkedCalc(ws, Var::abs_xsec_agenda_checked(ws),
                              Var::abs_species(ws), Var::abs_xsec_agenda(ws),
                              Var::verbosity(ws));
}

/*! Calculate absorption cross sections per tag group for HITRAN CIA continua.

This interpolates the cross sections from *abs_cia_data*.

The robust option is intended only for testing. Do not use for normal
runs, since subsequent functions will not be able to deal with NAN values.

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace
@param[in] T_extrapolfac - Temperature extrapolation factor (relative to grid
spacing). (default: 0.5)
@param[in] robust - Set to 1 to suppress runtime errors (and return NAN values
instead). (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_xsec_per_speciesAddCIA(Workspace& ws,
                                const Numeric& T_extrapolfac = 0.5,
                                const Index& robust = 0) {
  abs_xsec_per_speciesAddCIA(
      Var::abs_xsec_per_species(ws), Var::dabs_xsec_per_species_dx(ws),
      Var::abs_species(ws), Var::jacobian_quantities(ws),
      Var::abs_species_active(ws), Var::f_grid(ws), Var::abs_p(ws),
      Var::abs_t(ws), Var::abs_vmrs(ws), Var::abs_cia_data(ws), T_extrapolfac,
      robust, Var::verbosity(ws));
}

/*! Calculate absorption cross sections per tag group for continua.

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_xsec_per_speciesAddConts(Workspace& ws) {
  abs_xsec_per_speciesAddConts(
      Var::abs_xsec_per_species(ws), Var::dabs_xsec_per_species_dx(ws),
      Var::abs_species(ws), Var::jacobian_quantities(ws),
      Var::abs_species_active(ws), Var::f_grid(ws), Var::abs_p(ws),
      Var::abs_t(ws), Var::abs_vmrs(ws), Var::abs_cont_names(ws),
      Var::abs_cont_parameters(ws), Var::abs_cont_models(ws),
      Var::verbosity(ws));
}

/*! Calculate absorption cross sections per tag group for HITRAN xsec species.

This broadens the cross section data from *hitran_xsec_data* and
interpolates it onto the current f_grid.

apply_tfit turns of the temperature fit. It is only meant for testing
and should alwasy be kept on for real calculations.

This method depends on the FFTW-3 library.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] apply_tfit - Apply temperature fit. (default: 1)
@param[in] force_p - Positive value forces constant pressure [Pa]. (default: -1)
@param[in] force_t - Positive value forces constant temperature [K]. (default:
-1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_xsec_per_speciesAddHitranXsec(Workspace& ws,
                                       const Index& apply_tfit = 1,
                                       const Numeric& force_p = -1,
                                       const Numeric& force_t = -1) {
  abs_xsec_per_speciesAddHitranXsec(
      Var::abs_xsec_per_species(ws), Var::dabs_xsec_per_species_dx(ws),
      Var::abs_species(ws), Var::jacobian_quantities(ws),
      Var::abs_species_active(ws), Var::f_grid(ws), Var::abs_p(ws),
      Var::abs_t(ws), Var::hitran_xsec_data(ws), apply_tfit, force_p, force_t,
      Var::verbosity(ws));
}

/*! Calculates the line spectrum for both attenuation and phase
for each tag group and adds it to abs_xsec_per_species.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_xsec_per_speciesAddLines(Workspace& ws) {
  abs_xsec_per_speciesAddLines(
      Var::abs_xsec_per_species(ws), Var::src_xsec_per_species(ws),
      Var::dabs_xsec_per_species_dx(ws), Var::dsrc_xsec_per_species_dx(ws),
      Var::abs_species(ws), Var::jacobian_quantities(ws),
      Var::abs_species_active(ws), Var::f_grid(ws), Var::abs_p(ws),
      Var::abs_t(ws), Var::abs_nlte(ws), Var::abs_vmrs(ws),
      Var::abs_lines_per_species(ws), Var::isotopologue_ratios(ws),
      Var::partition_functions(ws), Var::lbl_checked(ws), Var::verbosity(ws));
}

/*! Reimplementation of published O2 absorption line cross-section algorithm

Based on:
        Dmitriy S. Makarov, Mikhail Yu. Tretyakov, Philip W. Rosenkranz, JQSRT
243, 2020, Revision of the 60-GHz atmospheric oxygen absorption band models for
practical use, https://doi.org/10.1016/j.jqsrt.2019.106798

Note that this is only really applicable to Earth and at lower altitudes.
The only two tested derivatives are for frequency and for temperature but
other untested derivatives are available for all model parameters except a2

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_xsec_per_speciesAddPredefinedO2MPM2020(Workspace& ws) {
  abs_xsec_per_speciesAddPredefinedO2MPM2020(
      Var::abs_xsec_per_species(ws), Var::dabs_xsec_per_species_dx(ws),
      Var::abs_species(ws), Var::jacobian_quantities(ws), Var::f_grid(ws),
      Var::abs_p(ws), Var::abs_t(ws), Var::abs_vmrs(ws), Var::verbosity(ws));
}

/*! Initialize *abs_xsec_per_species*.

The initialization is
necessary, because methods *abs_xsec_per_speciesAddLines*
and *abs_xsec_per_speciesAddConts* just add to *abs_xsec_per_species*.
The size is determined from *abs_species*.

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void abs_xsec_per_speciesInit(Workspace& ws) {
  abs_xsec_per_speciesInit(
      Var::abs_xsec_per_species(ws), Var::src_xsec_per_species(ws),
      Var::dabs_xsec_per_species_dx(ws), Var::dsrc_xsec_per_species_dx(ws),
      Var::abs_species(ws), Var::jacobian_quantities(ws),
      Var::abs_species_active(ws), Var::f_grid(ws), Var::abs_p(ws),
      Var::abs_xsec_agenda_checked(ws), Var::nlte_do(ws), Var::verbosity(ws));
}

/*! Sets up a gaussian antenna response.

The method assumes that the response is the same for all
frequencies and polarisations, and that it can be modelled as
gaussian.

The grid generated is approximately
   si * [-xwidth_si:dx_si:xwidth_si]
where si is the standard deviation corresponding to the FWHM.
That is, width and spacing of the grid is specified in terms of
number of standard deviations. If xwidth_si is set to 2, the
response will cover about 95% the complete response. For
xwidth_si=3, about 99% is covered. If xwidth_si/dx_si is not
an integer, the end points of the grid are kept and the spacing
of the grid is reduced (ie. spacing is equal or smaller *dx_si*).

If the 2D option is selected (*do_2d*), a circular antenna is
assumed and the response is any direction follows the 1D case.

The antenna repsonse is not normalised.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] fwhm - Full width at half-maximum
@param[in] xwidth_si - Half-width of response, in terms of std. dev. (default:
3)
@param[in] dx_si - Grid spacing, in terms of std. dev. (default: 0.1)
@param[in] do_2d - Set to 1 to create a 2D antenna pattern. (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void antenna_responseGaussian(Workspace& ws, const Numeric& fwhm,
                              const Numeric& xwidth_si = 3,
                              const Numeric& dx_si = 0.1,
                              const Index& do_2d = 0) {
  antenna_responseGaussian(Var::antenna_response(ws), fwhm, xwidth_si, dx_si,
                           do_2d, Var::verbosity(ws));
}

/*! Sets up gaussian antenna responses.

Similar to *antenna_responseGaussian* but allows to set up
responses that varies with frequency. That is, the method assumes
that the response is the same for all polarisations, and that it
can be modelled as a gaussian function varying with frequency.

The full width at half maximum (FWHM in radians) is calculated as:
    fwhm = lambda / leff
where lambda is the wavelength and *leff* is the effective size of
the antenna. Normally, *leff* is smaller than the physical antenna
size.

Antenna responses are created for *nf* frequencies spanning the
range [*fstart*,*fstop*], with a logarithmic spacing. That is, the
frequency grid of the responses is taken from *VectorNLogSpace*.

The responses have a common angular grid. The width, determined by
*xwidth_si*, is set for the lowest frequency, while the spacing
(*dx_si*) is set for the highest frequency. This ensures that both
the width and spacing are equal or better than *xwidth_si* and
*dx_si*, respectively, for all frequencies.

If the 2D option is selected (*do_2d*), a circular antenna is
assumed and the response is any direction follows the 1D case.

The antenna repsonse is not normalised.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] leff - Effective size of the antenna
@param[in] xwidth_si - Half-width of response, in terms of std. dev. (default:
3)
@param[in] dx_si - Grid spacing, in terms of std. dev. (default: 0.1)
@param[in] nf - Number of points in frequency grid (must be >= 2)
@param[in] fstart - Start point of frequency grid
@param[in] fstop - End point of frequency grid
@param[in] do_2d - Set to 1 to create a 2D antenna pattern. (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void antenna_responseVaryingGaussian(Workspace& ws, const Numeric& leff,
                                     const Index& nf, const Numeric& fstart,
                                     const Numeric& fstop,
                                     const Numeric& xwidth_si = 3,
                                     const Numeric& dx_si = 0.1,
                                     const Index& do_2d = 0) {
  antenna_responseVaryingGaussian(Var::antenna_response(ws), leff, xwidth_si,
                                  dx_si, nf, fstart, fstop, do_2d,
                                  Var::verbosity(ws));
}

/*! Adds a constant field to atm_fields_compact.

This is handy, e.g., for nitrogen or oxygen. The constant value can
be appended or prepended as an additional field to the already
existing collection of fields. All dimensions (pressure, latitude,
longitude) are filled up, so this works for 1D, 2D, or 3D
atmospheres.

The passed *name* of the field has to be in accordance with the
tagging structure described for *atm_fields_compact*.

A list of condensibles can be optionally specified if the VMR of
the added species is assuming dry air. The VMR of the added species
is then scaled down by the sum of the condensibles' VMR:
VMR * (1 - VMR_sum_of_condensibles).
For Earth this should be set to ["abs_species-H2O"]

@author Stefan Buehler, Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] name - Name of additional atmospheric field, with constant value.
@param[in] value - Constant value of additional field.
@param[in] prepend - 0 = Append to the end, 1 = insert at the beginning.
(default: 0)
@param[in] condensibles - List of condensibles used to scale down the VMR of the
added species. (default: {})

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void atm_fields_compactAddConstant(Workspace& ws, const String& name,
                                   const Numeric& value,
                                   const Index& prepend = 0,
                                   const ArrayOfString& condensibles = {}) {
  atm_fields_compactAddConstant(Var::atm_fields_compact(ws), name, value,
                                prepend, condensibles, Var::verbosity(ws));
}

/*! Adds a field to atm_fields_compact, with interpolation.

This method appends or prepends a *GriddedField3* to *atm_fields_compact*.
The *GriddedField3* is interpolated upon the grid of
*atm_fields_compact*. A typical use case for this method may be to
add a climatology of some gas when this gas is needed for radiative
transfer calculations, but not yet present in *atm_fields_compact*.
One case where this happens is when using the Chevalier91L dataset
for infrared simulations.

The grids in *atm_fields_compact* must fully encompass the grids in
the *GriddedField3* to be added, for interpolation to succeed. If
this is not the case, a RuntimeError is thrown.

The passed *name* of the field has to be in accordance with the
tagging structure described for *atm_fields_compact*.

@author Gerrit Holl

@param[in,out] Workspace ws - An ARTS workspace
@param[in] name - Name of additional atmospheric field.
@param[in] value - Value of additional atmospheric field.
@param[in] prepend - 0 = Append to the end, 1 = insert at the beginning.
(default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void atm_fields_compactAddSpecies(Workspace& ws, const String& name,
                                  const GriddedField3& value,
                                  const Index& prepend = 0) {
  atm_fields_compactAddSpecies(Var::atm_fields_compact(ws), name, value,
                               prepend, Var::verbosity(ws));
}

/*! Removes unrealistically small or erroneous data from
*atm_fields_compact* (or other GriddedField4 data)

This WSM checks if the data in *atm_fields_compact* contains
values smaller than the given *threshold*. In this case, these
values will be set to zero.

The method should be applied if *atm_fields_compact* contains
unrealistically small or erroneous data (NWP/GCM model data
occassionally contains negative values, which are numerical
artefacts rather than physical values.)

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] threshold - Threshold below which *atm_fields_compact* values are set
to zero.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void atm_fields_compactCleanup(Workspace& ws, const Numeric& threshold) {
  atm_fields_compactCleanup(Var::atm_fields_compact(ws), threshold,
                            Var::verbosity(ws));
}

/*! Initiates *atm_fields_compact* from a field.

*atm_fields_compact* will have the same size and grids as the GriddedField3,
but with one dimension as length 1.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] name - Name atmospheric field.
@param[in] field - The atmospheric field.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void atm_fields_compactCreateFromField(Workspace& ws, const String& name,
                                       const GriddedField3& field) {
  atm_fields_compactCreateFromField(Var::atm_fields_compact(ws), name, field,
                                    Var::verbosity(ws));
}

/*! Sets *atm_fields_compact* from 1D fields given in form of a matrix.

For batch calculations it is handy to store atmospheric
profiles in an array of matrix. We take such a matrix, and create
*atm_fields_compact* from it.

The matrix must contain one row for each pressure level.

Not all fields contained in the matrix must be selected into
*atm_fields_compact*, but the selection must at least contain
fields of pressure, temperature, altitude and one absorption
species.
The matrix can contain some additional fields which are not
directly used by ARTS for calculations but can be required for
further processing, e.g. wind speed and direction. These fields do
not need to be transfered into the *atm_fields_compact* variable.

Selection of fields into *atm_fields_compact* works by providing a
field name tag in *field_names* for the selected fields, while
unselected fields are tagged by 'ignore'. Order of tags in
*field_names* is strictly taken as corresponding to column order in
the matrix.
The pressure fields are by convention the first column of the
matrix, hence must not be tagged. That is, there must be given one
field name tag less than matrix columns.

For detailed tagging conventions see *atm_fields_compact*.

Works only for *atmosphere_dim==1.*

@author Stefan Buehler
@author Daniel Kreyling
@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] gin1 - One atmosphere matrix from batch input ArrayOfMatrix.
@param[in] field_names - Order/names of atmospheric fields.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void atm_fields_compactFromMatrix(Workspace& ws, const Matrix& gin1,
                                  const ArrayOfString& field_names) {
  atm_fields_compactFromMatrix(Var::atm_fields_compact(ws),
                               Var::atmosphere_dim(ws), gin1, field_names,
                               Var::verbosity(ws));
}

/*! Calls *p_gridFromZRaw*, *lat_gridFromZRaw* and *lon_gridFromZRaw*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] no_negZ - Exclude negative altitudes. (default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void atm_gridsFromZRaw(Workspace& ws, const Index& no_negZ = 1) {
  atm_gridsFromZRaw(Var::p_grid(ws), Var::lat_grid(ws), Var::lon_grid(ws),
                    Var::z_field_raw(ws), no_negZ, Var::verbosity(ws));
}

/*! Checks consistency of (clear sky) atmospheric fields.

The following WSVs are treated: *p_grid*, *lat_grid*, *lon_grid*,
*t_field*, *vmr_field*, wind_u/v/w_field and mag_u/v/w_field.

If any of the variables above is changed, then this method shall be
called again (no automatic check that this is fulfilled!).

The tests include that:
 1. Atmospheric grids (p/lat/lon_grid) are OK with respect to
    *atmosphere_dim* (and vmr_field also regarding *abs_species*).
 2. Atmospheric fields have sizes consistent with the atmospheric
    grids.
 3. *abs_f_interp_order* is not zero if any wind is nonzero.
 4. All values in *t_field* are > 0.

Default is that values in *vmr_field* are demanded to be >= 0
(ie. zero allowed, in contrast to *t_field*), but this
requirement can be removed by the *negative_vmr_ok* argument.

If any test fails, there is an error. Otherwise,
*atmfields_checked* is set to 1.

The cloudbox is covered by *cloudbox_checked*, *z_field* is
part of the checks done around *atmgeom_checked*.

If you choose to use *bad_partition_functions_ok* please note that
this is done on your own risk and that it could introduce hard-to-
track errors into your calculations.  Do not use this for anything
important.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] negative_vmr_ok - Flag whether to accept vmr_field < 0. (default: 0)
@param[in] bad_partition_functions_ok - Flag whether to accept partition
functions not covering *t_field* range. (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void atmfields_checkedCalc(Workspace& ws, const Index& negative_vmr_ok = 0,
                           const Index& bad_partition_functions_ok = 0) {
  atmfields_checkedCalc(
      Var::atmfields_checked(ws), Var::atmosphere_dim(ws), Var::p_grid(ws),
      Var::lat_grid(ws), Var::lon_grid(ws), Var::abs_species(ws),
      Var::t_field(ws), Var::vmr_field(ws), Var::wind_u_field(ws),
      Var::wind_v_field(ws), Var::wind_w_field(ws), Var::mag_u_field(ws),
      Var::mag_v_field(ws), Var::mag_w_field(ws), Var::partition_functions(ws),
      Var::abs_f_interp_order(ws), negative_vmr_ok, bad_partition_functions_ok,
      Var::verbosity(ws));
}

/*! Checks consistency of geometric considerations of the atmosphere.

The following WSVs are checked: *z_field*, *refellipsoid*, *z_surface*,
*lat_true* and *lon_true*. If any of the variables above is changed,
then this method shall be called again (no automatic check that this is
fulfilled!).

The tests include that:
 1. *refellipsoid* has correct size, and that eccentricity is
    set to zero if 1D atmosphere.
 2. *z_field* and *z_surface* have sizes consistent with the
    atmospheric grids.
 3. There is no gap between *z_surface* and *z_field*.

*lat_true* and *lon_true* are allowed to be empty.

If any test fails, there is an error. Otherwise, *atmgeom_checked*
is set to 1.

See further *atmgeom_checkedCalc*.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void atmgeom_checkedCalc(Workspace& ws) {
  atmgeom_checkedCalc(Var::atmgeom_checked(ws), Var::atmosphere_dim(ws),
                      Var::p_grid(ws), Var::lat_grid(ws), Var::lon_grid(ws),
                      Var::z_field(ws), Var::refellipsoid(ws),
                      Var::z_surface(ws), Var::lat_true(ws), Var::lon_true(ws),
                      Var::verbosity(ws));
}

/*! Calculates the averaging kernel matrix describing the sensitivity of the
OEM retrieval with respect to the true state of the system. A prerequisite
for the calculation of the averaging kernel matrix is a successful OEM
calculation in which the jacobian and the gain matrix dxdy have been calculated.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void avkCalc(Workspace& ws) {
  avkCalc(Var::avk(ws), Var::dxdy(ws), Var::jacobian(ws), Var::verbosity(ws));
}

/*! Sets up a rectangular channel response.

The response of the backend channels is hee assumed to be constant
inside the resolution width, and zero outside.

The method assumes that all channels have the same response.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] resolution - The spectrometer resolution.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void backend_channel_responseFlat(Workspace& ws, const Numeric& resolution) {
  backend_channel_responseFlat(Var::backend_channel_response(ws), resolution,
                               Var::verbosity(ws));
}

/*! Sets up a gaussian backend channel response.

The method assumes that all channels can be modelled as gaussian.

If *fwhm* has only one element, all channels are assumed to be equal.
If *fwhm* has multiple elements, *xwidth_si* and *dx_si* must have one
element or the same number of elements as *fwhm*. If one element is given,
this value will be used for all channels.

The grid generated can be written as
   si * [-xwidth_si:dx_si:xwidth_si]
where si is the standard deviation corresponding to the FWHM.
That is, width and spacing of the grid is specified in terms of
number of standard deviations. If xwidth_si is set to 2, the
response will cover about 95% the complete response. For
xwidth_si=3, about 99% is covered. If xwidth_si/dx_si is not
an integer, the end points of the grid are kept and the spacing
if the grid is adjusted in the downward direction (ie. spacing is.
is max *dx_si*).

@author Patrick Eriksson, Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] fwhm - Full width at half-maximum
@param[in] xwidth_si - Half-width of response, in terms of std. dev. (default:
{3})
@param[in] dx_si - Grid spacing, in terms of std. dev. (default: {0.1})

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void backend_channel_responseGaussian(Workspace& ws, const Vector& fwhm,
                                      const Vector& xwidth_si = {3},
                                      const Vector& dx_si = {0.1}) {
  backend_channel_responseGaussian(Var::backend_channel_response(ws), fwhm,
                                   xwidth_si, dx_si, Var::verbosity(ws));
}

/*! Adds a constant field to batch_atm_fields_compact.

Applies *atm_fields_compactAddConstant* to each batch.
The format is equal to that WSM.

@author Gerrit Holl

@param[in,out] Workspace ws - An ARTS workspace
@param[in] name - Name of additional atmospheric field, with constant value.
@param[in] value - Constant value of additional field.
@param[in] prepend - 0 = Append to the end, 1 = insert at the beginning.
(default: 0)
@param[in] condensibles - List of condensibles used to scale down the VMR of the
added species. (default: {})

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void batch_atm_fields_compactAddConstant(
    Workspace& ws, const String& name, const Numeric& value,
    const Index& prepend = 0, const ArrayOfString& condensibles = {}) {
  batch_atm_fields_compactAddConstant(Var::batch_atm_fields_compact(ws), name,
                                      value, prepend, condensibles,
                                      Var::verbosity(ws));
}

/*! Adds a field to *batch_atm_fields_compact*, with interpolation.

This method appends or prepends a *GriddedField3* to each *atm_fields_compact*.
in *batch_atm_fields_compact*. For details, see *atm_fields_compactAddSpecies*.

@author Gerrit Holl

@param[in,out] Workspace ws - An ARTS workspace
@param[in] name - Name of additional atmospheric field. Use, e.g., vmr_ch4 for
methane VMR
@param[in] value - Value of additional atmospheric field.
@param[in] prepend - 0 = Append to the end, 1 = insert at the beginning.
(default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void batch_atm_fields_compactAddSpecies(Workspace& ws, const String& name,
                                        const GriddedField3& value,
                                        const Index& prepend = 0) {
  batch_atm_fields_compactAddSpecies(Var::batch_atm_fields_compact(ws), name,
                                     value, prepend, Var::verbosity(ws));
}

/*! Removes unrealistically small or erroneous data from each data field
of *batch_atm_fields_compact* (or other AerrayOfGriddedField4 data)

This WSM checks if the data in *batch_atm_fields_compact* contains
values smaller than the given *threshold*. In this case, these
values will be set to zero.

The method should be applied if *batch_atm_fields_compact* contains
unrealistically small or erroneous data (NWP/GCM model data
occassionally contains negative values, which are numerical
artefacts rather than physical values.)

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] threshold - Threshold below which *atm_fields_compact* values are set
to zero.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void batch_atm_fields_compactCleanup(Workspace& ws, const Numeric& threshold) {
  batch_atm_fields_compactCleanup(Var::batch_atm_fields_compact(ws), threshold,
                                  Var::verbosity(ws));
}

/*! Expand batch of 1D atmospheric state matrices to batch_atm_fields_compact.

This is used to handle 1D batch cases, e.g. from NWP/GCM model like
the Chevallier91L data set, stored in a matrix (it is preferred,
though, to immediatedly store the model fields as
*ArrayOfGriddedField4* and use *ReadXML* to load them directly into
*batch_atm_fields_compact*).

Works only for *atmosphere_dim==1.*

See *atm_fields_compactFromMatrix* for basic documentation.

See *batch_atm_fields_compactAddConstant* and
batch_atm_fields_compactAddSpecies* for adding additional fields.

@author Stefan Buehler
@author Daniel Kreyling
@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] atmospheres_fields - Batch of atmospheres stored in one array of
matrix
@param[in] field_names - Order/names of atmospheric fields.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void batch_atm_fields_compactFromArrayOfMatrix(
    Workspace& ws, const ArrayOfMatrix& atmospheres_fields,
    const ArrayOfString& field_names) {
  batch_atm_fields_compactFromArrayOfMatrix(
      Var::batch_atm_fields_compact(ws), Var::atmosphere_dim(ws),
      atmospheres_fields, field_names, Var::verbosity(ws));
}

/*! Deactivates the cloud box.

Use this method if no scattering calculations shall be performed.

The function sets *cloudbox_on* to 0, *cloudbox_limits*,
*pnd_field*, *scat_data*, *scat_data_raw*, *iy_cloudbox_agenda*
and *particle_masses* to be empty and sizes *dpnd_field_dx* to be
consitent with *jacobian_quantities*.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void cloudboxOff(Workspace& ws) {
  cloudboxOff(Var::cloudbox_on(ws), Var::ppath_inside_cloudbox_do(ws),
              Var::cloudbox_limits(ws), Var::iy_cloudbox_agenda(ws),
              Var::pnd_field(ws), Var::dpnd_field_dx(ws), Var::scat_species(ws),
              Var::scat_data(ws), Var::scat_data_raw(ws),
              Var::scat_data_checked(ws), Var::particle_masses(ws),
              Var::jacobian_quantities(ws), Var::verbosity(ws));
}

/*! Sets the cloud box to encompass the cloud given by the entries
in *particle_field*.

This WSM handles one *Tensor4* type *particle_field* at a time. It can
be used to determine the cloudbox from *particle_bulkprop_field*,
but also from the various scat_species_XXX_field (or even from a
read-in *pnd_field*). For the latter, the WSM needs to be called
once per each scat_species_XXX_field, with previously determined
*cloudbox_limits* provided through *cloudbox_limits_old*.

The function must be called before executing any WSM that applies
*cloudbox_limits*.

The function iterates over all 3D fields in *particle_field* (which
might correspond to the different scattering species as in
scat_species_XXX_field or to different particle bulk properties as
in *particle_bulkprop_field*). Each field is searched for the first
and last pressure index, where the value is unequal to zero. This
index is then copied to *cloudbox_limits*.
If *particle_field* is empty, the cloudbox is switched off
(*cloudbox_on*=0).

Additionaly the lower cloudbox_limit is altered by *cloudbox_margin*.
The margin is given as a height difference in meters and transformed
into a pressure (via isothermal barometric height formula). This
alteration is to ensure covering photons that leave the cloud, but
reenter through a limb path.
If *cloudbox_margin* is set to -1 (default), the cloudbox will extend
to the surface. Hence, the lower cloudbox_limit is set to 0 (index
of first pressure level).
*cloudbox_margin* will be applied on each call of the WSM. Hence,
if called successively, e.g. for several scat_species_XXX_field,
and *cloudbox_margin* is not -1, it is suggested to apply the
desired *cloudbox_margin* only for the last WSM call, while for the
others set *cloudbox_margin* to 0.

Works only for *atmosphere_dim==1.*

@author Jana Mendrok, Daniel Kreyling

@param[in,out] Workspace ws - An ARTS workspace
@param[in] particle_field - A collection of particle property fields (e.g.
*particle_bulkprop_field*, *scat_species_mass_density_field*).
@param[in] cloudbox_limits_old - Preset cloudbox limits, e.g. resulting from a
previous run of *cloudboxSetAutomatically*. (default: {-1})
@param[in] cloudbox_margin - Minimum distance [m] between lowest 'cloudy' level
and cloudbox lower limit. If set to *-1* (default), the cloudbox lower limit is
fixed to 0, i.e., corresponds to the lowest atmospheric level (or the surface).
(default: -1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void cloudboxSetAutomatically(Workspace& ws, const Tensor4& particle_field,
                              const ArrayOfIndex& cloudbox_limits_old = {-1},
                              const Numeric& cloudbox_margin = -1) {
  cloudboxSetAutomatically(
      Var::cloudbox_on(ws), Var::cloudbox_limits(ws), Var::atmosphere_dim(ws),
      Var::p_grid(ws), Var::lat_grid(ws), Var::lon_grid(ws), particle_field,
      cloudbox_limits_old, cloudbox_margin, Var::verbosity(ws));
}

/*! Sets the cloudbox to cover the full atmosphere.

@author Claudia Emde, Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void cloudboxSetFullAtm(Workspace& ws) {
  cloudboxSetFullAtm(Var::cloudbox_on(ws), Var::cloudbox_limits(ws),
                     Var::atmosphere_dim(ws), Var::p_grid(ws),
                     Var::lat_grid(ws), Var::lon_grid(ws), Var::verbosity(ws));
}

/*! Sets the cloud box to encompass the given positions.

The function sets *cloudbox_on* to 1 and sets *cloudbox_limits*
following the given pressure, latitude and longitude positions.
The index limits in *cloudbox_limits* are selected to give the
smallest possible cloud box that encompass the given points.

The points must be given in the same order as used in
*cloudbox_limits*. That means that the first keyword argument
shall be a higher pressure than argument two, while the latitude
and longitude points are given in increasing order. Positions
given for dimensions not used by the selected atmospheric
dimensionality are ignored.

The given pressure points can be outside the range of *p_grid*.
The pressure limit is then set to the end point of *p_grid*.
The given latitude and longitude points must be inside the range
of the corresponding grid. In addition, the latitude and longitude
points cannot be inside the outermost grid ranges as the latitude
and longitude limits in *cloudbox_limits* are not allowed to be
grid end points.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] p1 - Upper pressure point.
@param[in] p2 - Lower pressure point.
@param[in] lat1 - Lower latitude point.
@param[in] lat2 - Upper latitude point.
@param[in] lon1 - Lower longitude point.
@param[in] lon2 - Upper longitude point.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void cloudboxSetManually(Workspace& ws, const Numeric& p1, const Numeric& p2,
                         const Numeric& lat1, const Numeric& lat2,
                         const Numeric& lon1, const Numeric& lon2) {
  cloudboxSetManually(Var::cloudbox_on(ws), Var::cloudbox_limits(ws),
                      Var::atmosphere_dim(ws), Var::p_grid(ws),
                      Var::lat_grid(ws), Var::lon_grid(ws), p1, p2, lat1, lat2,
                      lon1, lon2, Var::verbosity(ws));
}

/*! Sets the cloud box to encompass the given positions.

As *cloudboxSetManually* but uses altitudes instead of pressure.
The given altitude points can be outside the range of *z_field*.
The altitude limit is then set to the end point of *p_grid*.

@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace
@param[in] z1 - Lower altitude point.
@param[in] z2 - Upper altitude point.
@param[in] lat1 - Lower latitude point.
@param[in] lat2 - Upper latitude point.
@param[in] lon1 - Lower longitude point.
@param[in] lon2 - Upper longitude point.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void cloudboxSetManuallyAltitude(Workspace& ws, const Numeric& z1,
                                 const Numeric& z2, const Numeric& lat1,
                                 const Numeric& lat2, const Numeric& lon1,
                                 const Numeric& lon2) {
  cloudboxSetManuallyAltitude(Var::cloudbox_on(ws), Var::cloudbox_limits(ws),
                              Var::atmosphere_dim(ws), Var::z_field(ws),
                              Var::lat_grid(ws), Var::lon_grid(ws), z1, z2,
                              lat1, lat2, lon1, lon2, Var::verbosity(ws));
}

/*! Checks consistency and validity of the cloudbox governing variables.

The following WSVs are treated: *cloudbox_on*, *cloudbox_limits*,
*pnd_field*, *scat_data*, *scat_species*, *abs_species*, *particle_masses*
*particle_bulkprop_field*, *particle_bulkprop_names* and wind_u/v/w_field.

If any of these variables is changed, then this method shall be
called again (no automatic check that this is fulfilled!).

The main checks are if the cloudbox limits are OK with respect to
the atmospheric dimensionality and the limits of the atmosphere,
and that the scattering element variables *pnd_field* and
*scat_data* match in size.

Further checks on *scat_data* are performed in *scat_data_checkedCalc*

*scat_species* and *particle_masses* must either be empty or have a
size that matches the other data. If non-empty, some check of these
variables are performed.

If any test fails, there is an error. Otherwise, *cloudbox_checked*
is set to 1.

@author Patrick Eriksson, Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] negative_pnd_ok - Flag whether to accept pnd_field < 0. (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void cloudbox_checkedCalc(Workspace& ws, const Index& negative_pnd_ok = 0) {
  cloudbox_checkedCalc(
      Var::cloudbox_checked(ws), Var::atmfields_checked(ws),
      Var::atmosphere_dim(ws), Var::p_grid(ws), Var::lat_grid(ws),
      Var::lon_grid(ws), Var::z_field(ws), Var::z_surface(ws),
      Var::wind_u_field(ws), Var::wind_v_field(ws), Var::wind_w_field(ws),
      Var::cloudbox_on(ws), Var::cloudbox_limits(ws), Var::pnd_field(ws),
      Var::dpnd_field_dx(ws), Var::jacobian_quantities(ws), Var::scat_data(ws),
      Var::scat_species(ws), Var::particle_masses(ws), Var::abs_species(ws),
      negative_pnd_ok, Var::verbosity(ws));
}

/*! Extracts a part of an existing *cloudbox_field*.

The cropping is defined by defining new cloudbox limits. Note that
*new_limit0* is an index with respect to *p_grid*, etc.

The following must be valid:
  new_limit0 >= cloudbox_limits[0]
  new_limit1 <= cloudbox_limits[1]
  new_limit2 >= cloudbox_limits[2]
  new_limit3 <= cloudbox_limits[3]
  new_limit4 >= cloudbox_limits[4]
  new_limit5 <= cloudbox_limits[5]

Indexes for dimensions not used are ignored.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] new_limit0 - New value for cloudbox_limits[0]. (default: 0)
@param[in] new_limit1 - New value for cloudbox_limits[1]. (default: 0)
@param[in] new_limit2 - New value for cloudbox_limits[2]. (default: 0)
@param[in] new_limit3 - New value for cloudbox_limits[3]. (default: 0)
@param[in] new_limit4 - New value for cloudbox_limits[4]. (default: 0)
@param[in] new_limit5 - New value for cloudbox_limits[5]. (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void cloudbox_fieldCrop(Workspace& ws, const Index& new_limit0 = 0,
                        const Index& new_limit1 = 0,
                        const Index& new_limit2 = 0,
                        const Index& new_limit3 = 0,
                        const Index& new_limit4 = 0,
                        const Index& new_limit5 = 0) {
  cloudbox_fieldCrop(Var::cloudbox_field(ws), Var::cloudbox_limits(ws),
                     Var::atmosphere_dim(ws), Var::cloudbox_on(ws), new_limit0,
                     new_limit1, new_limit2, new_limit3, new_limit4, new_limit5,
                     Var::verbosity(ws));
}

/*! Interpolate clearsky field on all gridpoints in cloudbox.

This method uses a linear 1D/3D interpolation scheme to obtain the
radiation field on all grid points inside the cloud box from the
clear sky field on the cloudbox boundary. This radiation field
is taken as the first guess radiation field in the DOIT module.

Set the *all_frequencies* to 1 if the clearsky field shall be used
as initial field for all frequencies. Set it to 0 if the clear sky
field shall be used only for the first frequency in *f_grid*. For
later frequencies, *cloudbox_field* of the previous frequency is then
used.

@author Sreerekha T.R. and Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace
@param[in] all_frequencies - See above. (default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void cloudbox_fieldSetClearsky(Workspace& ws,
                               const Index& all_frequencies = 1) {
  cloudbox_fieldSetClearsky(Var::cloudbox_field(ws), Var::p_grid(ws),
                            Var::lat_grid(ws), Var::lon_grid(ws),
                            Var::cloudbox_limits(ws), Var::atmosphere_dim(ws),
                            Var::cloudbox_on(ws), Var::doit_is_initialized(ws),
                            all_frequencies, Var::verbosity(ws));
}

/*! This method sets the initial field inside the cloudbox to a
constant value.

The user has to specify a value for each Stokes dimension in the
control file by *value*.

@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace
@param[in] value - A vector containing *stokes_dim* elements with the value of
the initial field for each Stokes dimension.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void cloudbox_fieldSetConst(Workspace& ws, const Vector& value) {
  cloudbox_fieldSetConst(Var::cloudbox_field(ws), Var::p_grid(ws),
                         Var::lat_grid(ws), Var::lon_grid(ws),
                         Var::cloudbox_limits(ws), Var::atmosphere_dim(ws),
                         Var::stokes_dim(ws), value, Var::verbosity(ws));
}

/*! This method sets the initial field inside the cloudbox to a
constant value per frequency slice.

The user has specify a value for each frequency and Stokes
dimension in the control file by *value*.

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] value - A matrix containing *stokes_dim* elements per frequency (row)
with the value of the initial field for each frequency and Stokes dimension.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void cloudbox_fieldSetConstPerFreq(Workspace& ws, const Matrix& value) {
  cloudbox_fieldSetConstPerFreq(
      Var::cloudbox_field(ws), Var::p_grid(ws), Var::lat_grid(ws),
      Var::lon_grid(ws), Var::cloudbox_limits(ws), Var::atmosphere_dim(ws),
      Var::stokes_dim(ws), value, Var::verbosity(ws));
}

/*! Sets the initial cloudbox intensity field *cloudbox_field* from a
precalculated field.

This method sets the (monochromatic) first guess radiation field
inside the cloudbox from a precalculated *cloudbox_field_precalc*,
e.g., from the solution of a similar atmospheric scenario. The
dimensions of *cloudbox_field_precalc* have to be consistent with
the DOIT setup in terms of frequencies, pressure levels inside the
cloudbox, polar angles used as well as the stokes dimension.
Incoming field on the cloudbox boundaries is adapted to the actual
clearsky incoming field as, e.g., calculated by *DoitGetIncoming*.

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] cloudbox_field_precalc - Precalculated radiation field (of type
*cloudbox_field*)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void cloudbox_fieldSetFromPrecalc(Workspace& ws,
                                  const Tensor7& cloudbox_field_precalc) {
  cloudbox_fieldSetFromPrecalc(
      Var::cloudbox_field(ws), Var::za_grid(ws), Var::f_grid(ws),
      Var::atmosphere_dim(ws), Var::stokes_dim(ws), Var::cloudbox_limits(ws),
      Var::doit_is_initialized(ws), cloudbox_field_precalc, Var::verbosity(ws));
}

/*! RT calculation in cloudbox with fixed scattering integral (1D).

Updates the radiation field (DOIT method). The method loops
through the cloudbox to update the radiation field for all
positions and directions in the 1D cloudbox.

Note: This method is very inefficient, because the number of
iterations scales with the number of cloudbox pressure levels.
It is recommended to use *cloudbox_fieldUpdateSeq1D*.

@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void cloudbox_fieldUpdate1D(Workspace& ws) {
  cloudbox_fieldUpdate1D(
      ws, Var::cloudbox_field_mono(ws), Var::doit_scat_field(ws),
      Var::cloudbox_limits(ws), Var::propmat_clearsky_agenda(ws),
      Var::vmr_field(ws), Var::spt_calc_agenda(ws), Var::za_grid(ws),
      Var::pnd_field(ws), Var::ppath_step_agenda(ws), Var::ppath_lmax(ws),
      Var::ppath_lraytrace(ws), Var::p_grid(ws), Var::z_field(ws),
      Var::refellipsoid(ws), Var::t_field(ws), Var::f_grid(ws),
      Var::f_index(ws), Var::surface_rtprop_agenda(ws), Var::doit_za_interp(ws),
      Var::verbosity(ws));
}

/*! RT calculation in cloudbox with fixed scattering integral.

Updates radiation field (*cloudbox_field*) in DOIT module.
This method loops through the cloudbox to update the
radiation field for all positions and directions in the 1D
cloudbox. The method applies the sequential update. For more
information refer to AUG.

@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace
@param[in] normalize - Apply normalization to scattered field. (default: 1)
@param[in] norm_error_threshold - Error threshold for scattered field correction
factor. (default: 1.0)
@param[in] norm_debug - Debugging flag. Set to 1 to output normalization factor
to out0. (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void cloudbox_fieldUpdateSeq1D(Workspace& ws, const Index& normalize = 1,
                               const Numeric& norm_error_threshold = 1.0,
                               const Index& norm_debug = 0) {
  cloudbox_fieldUpdateSeq1D(
      ws, Var::cloudbox_field_mono(ws), Var::doit_scat_field(ws),
      Var::cloudbox_limits(ws), Var::propmat_clearsky_agenda(ws),
      Var::vmr_field(ws), Var::spt_calc_agenda(ws), Var::za_grid(ws),
      Var::aa_grid(ws), Var::pnd_field(ws), Var::ppath_step_agenda(ws),
      Var::ppath_lmax(ws), Var::ppath_lraytrace(ws), Var::p_grid(ws),
      Var::z_field(ws), Var::refellipsoid(ws), Var::t_field(ws),
      Var::f_grid(ws), Var::f_index(ws), Var::surface_rtprop_agenda(ws),
      Var::doit_za_interp(ws), normalize, norm_error_threshold, norm_debug,
      Var::verbosity(ws));
}

/*! RT calculation in cloudbox with fixed scattering integral.

 Update radiation field (*cloudbox_field*) in DOIT module.
This method loops through the cloudbox to update the
radiation field for all
positions and directions in the 1D cloudbox. The method applies
the sequential update and the plane parallel approximation.
This method is only slightly faster than
*cloudbox_fieldUpdateSeq1D* and it is less accurate. It can not
be used for limb simulations.

@author Sreerekha T.R.

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void cloudbox_fieldUpdateSeq1DPP(Workspace& ws) {
  cloudbox_fieldUpdateSeq1DPP(
      ws, Var::cloudbox_field_mono(ws), Var::za_index(ws),
      Var::doit_scat_field(ws), Var::cloudbox_limits(ws),
      Var::propmat_clearsky_agenda(ws), Var::vmr_field(ws),
      Var::spt_calc_agenda(ws), Var::za_grid(ws), Var::pnd_field(ws),
      Var::p_grid(ws), Var::z_field(ws), Var::t_field(ws), Var::f_grid(ws),
      Var::f_index(ws), Var::verbosity(ws));
}

/*! RT calculation in cloudbox with fixed scattering integral.

Update radiation field (*cloudbox_field*) in DOIT module.
This method loops through the cloudbox to update the
radiation field for all positions and directions in the 3D
cloudbox. The method applies the sequential update. For more
information please refer to AUG.
Surface reflections are not yet implemented in 3D scattering
calculations.

@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void cloudbox_fieldUpdateSeq3D(Workspace& ws) {
  cloudbox_fieldUpdateSeq3D(
      ws, Var::cloudbox_field_mono(ws), Var::doit_scat_field(ws),
      Var::cloudbox_limits(ws), Var::propmat_clearsky_agenda(ws),
      Var::vmr_field(ws), Var::spt_calc_agenda(ws), Var::za_grid(ws),
      Var::aa_grid(ws), Var::pnd_field(ws), Var::ppath_step_agenda(ws),
      Var::ppath_lmax(ws), Var::ppath_lraytrace(ws), Var::p_grid(ws),
      Var::lat_grid(ws), Var::lon_grid(ws), Var::z_field(ws),
      Var::refellipsoid(ws), Var::t_field(ws), Var::f_grid(ws),
      Var::f_index(ws), Var::doit_za_interp(ws), Var::verbosity(ws));
}

/*! Iterative solution of the VRTE (DOIT method).

A solution for the RTE with scattering is found using the
DOIT method:
 1. Calculate scattering integral using *doit_scat_field_agenda*.
 2. Calculate RT with fixed scattered field using
    *doit_rte_agenda*.
 3. Convergence test using *doit_conv_test_agenda*.

Note: The atmospheric dimensionality *atmosphere_dim* can be
      either 1 or 3. To these dimensions the method adapts
      automatically. 2D scattering calculations are not
      supported.

@author Claudia Emde, Jakob Doerr

@param[in,out] Workspace ws - An ARTS workspace
@param[in] accelerated - Index wether to accelerate only the intensity (1) or
the whole Stokes Vector (4) (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void cloudbox_field_monoIterate(Workspace& ws, const Index& accelerated = 0) {
  cloudbox_field_monoIterate(
      ws, Var::cloudbox_field_mono(ws), Var::doit_scat_field_agenda(ws),
      Var::doit_rte_agenda(ws), Var::doit_conv_test_agenda(ws), accelerated,
      Var::verbosity(ws));
}

/*! Interpolate *cloudbox_field_mono* back to the original p_grid.
For detailed description, see *OptimizeDoitPressureGrid*.

@author Jakob Doerr

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void cloudbox_field_monoOptimizeReverse(Workspace& ws) {
  cloudbox_field_monoOptimizeReverse(
      Var::cloudbox_field_mono(ws), Var::p_grid_orig(ws), Var::p_grid(ws),
      Var::cloudbox_limits(ws), Var::verbosity(ws));
}

/*! This method sets the initial field inside the cloudbox to a
constant value. The method works only for monochromatic
calculations (number of elements in f_grid=1).

The user can specify a value for each Stokes dimension in the
control file by *value*.

@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace
@param[in] value - A vector containing 4 elements with the value of the initial
field for each Stokes dimension.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void cloudbox_field_monoSetConst(Workspace& ws, const Vector& value) {
  cloudbox_field_monoSetConst(Var::cloudbox_field_mono(ws), Var::p_grid(ws),
                              Var::lat_grid(ws), Var::lon_grid(ws),
                              Var::cloudbox_limits(ws), Var::atmosphere_dim(ws),
                              Var::stokes_dim(ws), value, Var::verbosity(ws));
}

/*! Reads *collision_coefficients* and *collision_line_identifiers* from
location on filesystem with many species.  The species in this location must
match *abs_species*.  The location must also contain an ArrayOfQuantumIdentifier
file ending with qid.xml

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] basename - path to files to read (default: "./")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void collision_coefficientsFromSplitFiles(Workspace& ws,
                                          const String& basename = "./") {
  collision_coefficientsFromSplitFiles(
      Var::collision_coefficients(ws), Var::collision_line_identifiers(ws),
      Var::abs_species(ws), basename, Var::verbosity(ws));
}

/*! Set complex refractive index to a constant value.

Frequency and temperature grids are set to have length 1 (and
set to the value 0).

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] refr_index_real - Real part of refractive index
@param[in] refr_index_imag - Imag part of refractive index

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void complex_refr_indexConstant(Workspace& ws, const Numeric& refr_index_real,
                                const Numeric& refr_index_imag) {
  complex_refr_indexConstant(Var::complex_refr_index(ws), refr_index_real,
                             refr_index_imag, Var::verbosity(ws));
}

/*! Refractive index of ice following Matzler06 parameterization.

Calculates temperature dependent complex refractive index of
hexagonal ice at microwave and sub-mm frequencies (10MHz-3Tz).

This parametrization is also applied by the microwave and
submm-wave part of the Warren08 model.

References:
Matzler, C., 2006: Thermal Microwave Radiation: Application for
Remote Sensing, Microwave dielectric properties of ice, pp. 455-462,
Inst. Eng. Technol., Stevenage, U. K.
Warren, S. G., and R. E. Brandt, 2008: Optical constants of ice
from the ultraviolet to the microwave: A revised compilation,
J. Geophys. Res., 113, D14220, doi:10.1029/2007JD009744.

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] data_f_grid - Frequency grid for refractive index calculation
@param[in] data_T_grid - Temperature grid for refractive index calculation

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void complex_refr_indexIceMatzler06(Workspace& ws, const Vector& data_f_grid,
                                    const Vector& data_T_grid) {
  complex_refr_indexIceMatzler06(Var::complex_refr_index(ws), data_f_grid,
                                 data_T_grid, Var::verbosity(ws));
}

/*! Refractive index of ice following Warren84 parameterization.

Calculates complex refractive index of Ice 1H for wavelengths
between 45 nm and 8.6 m.
For wavelengths above 167 microns, temperature dependence is
included for temperatures between 213 and 272K.
Mainly intended for applications in Earth ice
clouds and snow, not other planets or interstellar space;
the temperature dependence or crystalline form of ice may be
incorrect for these latter applications.

Authors of Fortran function:
Stephen Warren, Univ. of Washington (1983)
Bo-Cai Gao, JCESS, Univ. of Maryland (1995)
Warren Wiscombe, NASA Goddard (1995)

References:
Warren, S., 1984: Optical Constants of Ice from the Ultraviolet
to the Microwave, Appl. Opt. 23, 1206-1225

Kou, L., D. Labrie, and P. Chylek, 1994: Refractive indices
of water and ice in the 0.65- to 2.5-micron spectral range,
Appl. Opt. 32, 3531-3540

Perovich, D., and J. Govoni, 1991: Absorption Coefficients
of Ice from 250 to 400 nm, Geophys. Res. Lett. 18, 1233-1235

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] data_f_grid - Frequency grid for refractive index calculation
@param[in] data_T_grid - Temperature grid for refractive index calculation

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void complex_refr_indexIceWarren84(Workspace& ws, const Vector& data_f_grid,
                                   const Vector& data_T_grid) {
  complex_refr_indexIceWarren84(Var::complex_refr_index(ws), data_f_grid,
                                data_T_grid, Var::verbosity(ws));
}

/*! Complex refractive index of liquid water according to Liebe 1993.

The method treats liquid water without salt. Thus, not valid below
10 GHz. Upper frequency limit not known, here set to 1000 GHz.
Model parameters taken from Atmlab function epswater93 (by
C. Maetzler), which refer to Liebe 1993 without closer
specifications.

Temperatures must be between -40 and 100 degrees Celsius. The
accuracy of the parametrization below 0 C is not known by us.

@author Patrick Eriksson
@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] data_f_grid - Frequency grid for refractive index calculation
@param[in] data_T_grid - Temperature grid for refractive index calculation

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void complex_refr_indexWaterLiebe93(Workspace& ws, const Vector& data_f_grid,
                                    const Vector& data_T_grid) {
  complex_refr_indexWaterLiebe93(Var::complex_refr_index(ws), data_f_grid,
                                 data_T_grid, Var::verbosity(ws));
}

/*! Create 1D covariance matrix.

Creates a 1D covariance matrix for two retrieval quantities on given
 grids from a given functional form. Elements  of the covariance matrix
are computed as
 S_{i,j} = sigma_i * sigma_j * f(d_{i,j} / l_{i,j})
 where d_{i,j} is the distance between the two grid points and l_{i,j}
 the mean of the correlation lengths of the grid points.

 If a cutoff value co is given elements with absolute value less than this
 are set to zero.

The following functional forms are available:
  "exp": f(x) = exp(-x)
  "lin": f(x) = 1.0 - x, for x > 1.0, 0.0 otherwise
  "gauss": f(x) = exp(-x^2)

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - The matrix in which to store the covariance matrix.
@param[in] grid_1 - The retrieval grid for the first retrieval quantity.
@param[in] grid_2 - The retrieval grid for the second retrieval quantity. (If
empty taken as grid_1) (default: {})
@param[in] sigma_1 - The variances of the first retrieval quantity.
@param[in] sigma_2 - The variances of the second retrieval quantity.(If empty
taken as sigma_1) (default: {})
@param[in] cls_1 - The correlations lengths of the first retrieval quantity.
@param[in] cls_2 - The correlations lengths of the second retrieval quantity.(If
empty taken as cls_1) (default: {})
@param[in] co - The cutoff value for covariance matrix elements. (default: 0.0)
@param[in] fname - The name of the functional form to use.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void covmat1D(Workspace& ws, Any_0& out, const Vector& grid_1,
              const Vector& sigma_1, const Vector& cls_1, const String& fname,
              const Vector& grid_2 = {}, const Vector& sigma_2 = {},
              const Vector& cls_2 = {}, const Numeric& co = 0.0) {
  covmat1D(out, grid_1, grid_2, sigma_1, sigma_2, cls_1, cls_2, co, fname,
           Var::verbosity(ws));
}

/*! Create Markov Process Covariance Matrix.

Create a markov process covariance matrix for a retrieval quantity on
 evenly spaced 1D grid. The correlation between two grid points i,j is
 is computed as
 cov(i,j) = sigma[i] * sigma[j] * exp(- d(i,j) / lc)
 where d(i,j) = abs(grid[i] - grid[j]).

This function also sets covmat_inv_block to the analytically computed inverse
of the covariance matrix of the markov provess, which is tri-diagonal. Note
that this requires the retrieval grid to be evenly spaced.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - The matrix in which to store the covariance matrix.
@param[out] out_inverse - The matrix in which to store the inverse of the
covariance matrix.
@param[in] grid - The retrieval grid.
@param[in] sigma - The vairance for each grid point.
@param[in] lc - The correlation length of the Markov process.
@param[in] co - The cutoff value below which elements will be set to 0.0
(default: 0.0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0, typename Any_1>
void covmat1DMarkov(Workspace& ws, Any_0& out, Any_1& out_inverse,
                    const Vector& grid, const Vector& sigma, const Numeric& lc,
                    const Numeric& co = 0.0) {
  covmat1DMarkov(out, out_inverse, grid, sigma, lc, co, Var::verbosity(ws));
}

/*! Sets the matrix in covmat_block to a diagonal matrix with the variances
provided in *vars* as diagonal elements.
Also sets covmat_block_inv to the inverse of the block so that the
computation of the inverse is avoided.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - The matrix in which to store the covariance matrix.
@param[out] out_inverse - The matrix in which to store the inverse of the
covariance matrix.
@param[in] vars - Variances to be used as diagonal elements of covmat_block.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0, typename Any_1>
void covmatDiagonal(Workspace& ws, Any_0& out, Any_1& out_inverse,
                    const Vector& vars) {
  covmatDiagonal(out, out_inverse, vars, Var::verbosity(ws));
}

/*! Add a block to the measurement covariance matrix *covmat_se*

This functions adds a given dense or sparse matrix as block to the covariance
matrix *covmat_sx*. The position of the block can be given by the generic
arguments *i* and *j*. Note that diagonal blocks must be added in order starting
from in  the top left corner. If an off-diagonal block is added it must have
corresponding existing blocks on the diagonal and these must be consistent with
the dimensions of the block.  If *i* and *j*  are not provided, the blok will be
added at the first free spot on the diagonal.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[in] block - The block to add to the covariance matrix
@param[in] i - Index of a retrieval quantity. Must satisfy *i* <= *j*. (default:
-1)
@param[in] j - Index of a retrieval quantity. Must satisfy *i* <= *j*. (default:
-1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void covmat_seAddBlock(Workspace& ws, const Any_0& block, const Index& i = -1,
                       const Index& j = -1) {
  covmat_seAddBlock(Var::covmat_se(ws), block, i, j, Var::verbosity(ws));
}

/*! Add the inverse of a block to covariance matrix *covmat_se*

This functions adds a given matrix as the inverse of a block in the covariance
matrix *covmat_se*. The purpose of this function is to allow the user to
to use a precomputed inverse for this block in the covariance matrix, that may
for example have been obtained analytically.

This function requires the corresponding non-inverse block to already be present
in *covmat_se*

 Note that for this to work this retrieval quantity must be independent from
other retrieval quantities that do not have an inverse. Otherwise the inverse
will be ignored and recomputed numerically.

For the rest, the same requirements as for *covmat_seAddBlock* apply.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[in] block - The inverse block to add to the covariance matrix
@param[in] i - Index of a retrieval quantity. Must satisfy *i* <= *j*. (default:
-1)
@param[in] j - Index of a retrieval quantity. Must satisfy *i* <= *j*. (default:
-1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void covmat_seAddInverseBlock(Workspace& ws, const Any_0& block,
                              const Index& i = -1, const Index& j = -1) {
  covmat_seAddInverseBlock(Var::covmat_se(ws), block, i, j, Var::verbosity(ws));
}

/*! Set covmat_se to a given matrix.

This sets the measurement covariance matrix *covmat_se* to
the matrix given by the generic input *covmat*. The covariance
matrix can be of type CovarianceMatrix, Matrix or Sparse.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[in] covmat - The matrix to set as the covariance matrix.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void covmat_seSet(Workspace& ws, const Any_0& covmat) {
  covmat_seSet(Var::covmat_se(ws), covmat, Var::verbosity(ws));
}

/*! Calculates the covariance matrix describing the error due to uncertainties
in the observation system. The uncertainties of the observation system are
described by *covmat_se*, which must be set by the user to include the
relevant contributions from the measurement and the forward model.

Prerequisite for the calculation of *covmat_so* is a successful OEM
computation where also the gain matrix has been computed.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void covmat_soCalc(Workspace& ws) {
  covmat_soCalc(Var::covmat_so(ws), Var::dxdy(ws), Var::covmat_se(ws),
                Var::verbosity(ws));
}

/*! Calculates the covariance matrix describing the error due to smoothing.
The calculation of *covmat_ss* also requires the averaging kernel matrix *avk*
to be computed after a successful OEM calculation.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void covmat_ssCalc(Workspace& ws) {
  covmat_ssCalc(Var::covmat_ss(ws), Var::avk(ws), Var::covmat_sx(ws),
                Var::verbosity(ws));
}

/*! Add a block to the a priori covariance matrix *covmat_sx*

This functions adds a given matrix as a block in the covariance
matrix *covmat_sx*. The position of the block can be given by the generic
arguments *i* and *j*, which should give the index of the retrieval quantity in
*jacobian_quantities*, which is given just by the order the quantities have been
added to the retrieval.

If arguments *i* and *j* are omitted, the block will be added as diagonal block
for the last added retrieval quantity.

If provided, the index *i* must be less than or equal to *j*. Also the provided
block must be consistent with the corresponding retrieval quantities.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[in] block - The block to add to the covariance matrix
@param[in] i - Index of a retrieval quantity. Must satisfy *i* <= *j*. (default:
-1)
@param[in] j - Index of a retrieval quantity. Must satisfy *i* <= *j*. (default:
-1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void covmat_sxAddBlock(Workspace& ws, const Any_0& block, const Index& i = -1,
                       const Index& j = -1) {
  covmat_sxAddBlock(Var::covmat_sx(ws), Var::jacobian_quantities(ws), block, i,
                    j, Var::verbosity(ws));
}

/*! Add the inverse of a block in covariance matrix *covmat_sx*

This functions adds a given matrix as the inverse of a block in the covariance
matrix *covmat_sx*. The purpose of this function is to allow the user to
to use a precomputed inverse for this block in the covariance matrix, the may
for example by obtained analytically.

This function requires the non-inverse block to already be present in
*covmat_sx*

 Note that for this to work this retrieval quantity must be independent from
other retrieval quantities that do not have an inverse. Otherwise the inverse
will be ignored and recomputed numerically.

For the rest, the same requirements as for *covmat_sxAddBlock* apply.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[in] block - The inverse block to add to the covariance matrix
@param[in] i - Index of a retrieval quantity. Must satisfy *i* <= *j*. (default:
-1)
@param[in] j - Index of a retrieval quantity. Must satisfy *i* <= *j*. (default:
-1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void covmat_sxAddInverseBlock(Workspace& ws, const Any_0& block,
                              const Index& i = -1, const Index& j = -1) {
  covmat_sxAddInverseBlock(Var::covmat_sx(ws), Var::jacobian_quantities(ws),
                           block, i, j, Var::verbosity(ws));
}

/*! Extract the square root of the diagonal of the state space covariance
matrix. This function extracts the diagonal of the state space covariance matrix
*covmat_sx* and computes its square root. The resulting vector can then
be used as *x_norm* argument for the OEM method to avoid scaling problems.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[out] x_norm - The vector containing the square root of the diagonal
elements of *covmat_sx*

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void covmat_sxExtractSqrtDiagonal(Workspace& ws, Vector& x_norm) {
  covmat_sxExtractSqrtDiagonal(x_norm, Var::covmat_sx(ws), Var::verbosity(ws));
}

/*! Set covmat_sx to a given matrix.

This sets the measurement covariance matrix *covmat_sx* to
the matrix given by the generic input *covmat*. The covariance
matrix can be of type CovarianceMatrix, Matrix or Sparse.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[in] covmat - The matrix to set as the covariance matrix.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void covmat_sxSet(Workspace& ws, const Any_0& covmat) {
  covmat_sxSet(Var::covmat_sx(ws), covmat, Var::verbosity(ws));
}

/*! Calculates maximum and area equivalent diameters from volume
equivalent diameter.

This is primarily a help function for using the T-matrix method
and only a few particle shapes are handled.
For shapes handled and further comments on the input arguments, see
*scat_data_singleTmatrix*.

Area equivalent diameter is the equivalent sphere diameter
corresponding to the "maximum axial area". This is the largest
cross-sectional area of the particle, observed either along the
particle's main axis or in the perpendicular direction. That is,
for a cylinder having diameter d and thickness h, this area is
either (pi*d^2)/4 or (h*d).

@author Johan Strandgren
@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] diameter_max - Maximum dimension of the particle.
@param[out] diameter_area_equ - Maximum axial area equivalent diameter of the
particle, see above.
@param[in] shape - Particle shape.
@param[in] diameter_volume_equ - Particle equivalent volume diameter.
@param[in] aspect_ratio - Particle aspect ratio.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void diameter_maxFromDiameter_volume_equ(Workspace& ws, Numeric& diameter_max,
                                         Numeric& diameter_area_equ,
                                         const String& shape,
                                         const Numeric& diameter_volume_equ,
                                         const Numeric& aspect_ratio) {
  diameter_maxFromDiameter_volume_equ(diameter_max, diameter_area_equ, shape,
                                      diameter_volume_equ, aspect_ratio,
                                      Var::verbosity(ws));
}

/*! Converts from maximum to volume equivalent diameter.

This is primarily a help function for using the T-matrix part
and only a few particle shapes are handled.
For shapes handled and further comments on the input arguments,
see *scat_data_singleTmatrix*.

Also the volume is provided. It is simply sqrt(pi*dveq^3/6).

@author Johan Strandgren
@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] diameter_volume_equ - Particle volume equivalent diameter.
@param[out] volume - Volume of the particle.
@param[in] shape - Particle shape.
@param[in] diameter_max - Maximum dimension of the particle.
@param[in] aspect_ratio - Particle aspect ratio.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void diameter_volume_equFromDiameter_max(Workspace& ws,
                                         Numeric& diameter_volume_equ,
                                         Numeric& volume, const String& shape,
                                         const Numeric& diameter_max,
                                         const Numeric& aspect_ratio) {
  diameter_volume_equFromDiameter_max(diameter_volume_equ, volume, shape,
                                      diameter_max, aspect_ratio,
                                      Var::verbosity(ws));
}

/*! DOIT convergence test (maximum absolute difference).

The function calculates the absolute differences for two successive
iteration fields. It picks out the maximum values for each Stokes
component separately. The convergence test is fullfilled under the
following conditions:
   |I(m+1) - I(m)| < epsilon_1     Intensity.
   |Q(m+1) - Q(m)| < epsilon_2     The other Stokes components.
   |U(m+1) - U(m)| < epsilon_3
   |V(m+1) - V(m)| < epsilon_4
These conditions have to be valid for all positions in the
cloudbox and for all directions.

@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace
@param[in] epsilon - Limits for convergence. A vector with length matching
*stokes_dim* with unit [W / (m^2 Hz sr)].
@param[in] max_iterations - Maximum number of iterations allowed to reach
convergencelimit. (default: 100)
@param[in] nonconv_return_nan - Flag whether to accept result at max_iterations
(0=default)or whether to return NaNs in case of non-convergence atmax_iterations
(default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void doit_conv_flagAbs(Workspace& ws, const Vector& epsilon,
                       const Index& max_iterations = 100,
                       const Index& nonconv_return_nan = 0) {
  doit_conv_flagAbs(Var::doit_conv_flag(ws), Var::doit_iteration_counter(ws),
                    Var::cloudbox_field_mono(ws),
                    Var::cloudbox_field_mono_old(ws), epsilon, max_iterations,
                    nonconv_return_nan, Var::verbosity(ws));
}

/*! DOIT convergence test (maximum absolute difference in Rayleigh Jeans BT)

As *doit_conv_flagAbs* but convergence limits are specified in
Rayleigh-Jeans brighntess temperatures.

@author Sreerekha T.R.
@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace
@param[in] epsilon - Limits for convergence. A vector with length matching
*stokes_dim* with unit [K].
@param[in] max_iterations - Maximum number of iterations allowed to reach
convergencelimit. (default: 100)
@param[in] nonconv_return_nan - Flag whether to accept result at max_iterations
(0=default)or whether to return NaNs in case of non-convergence atmax_iterations
(default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void doit_conv_flagAbsBT(Workspace& ws, const Vector& epsilon,
                         const Index& max_iterations = 100,
                         const Index& nonconv_return_nan = 0) {
  doit_conv_flagAbsBT(Var::doit_conv_flag(ws), Var::doit_iteration_counter(ws),
                      Var::cloudbox_field_mono(ws),
                      Var::cloudbox_field_mono_old(ws), Var::f_grid(ws),
                      Var::f_index(ws), epsilon, max_iterations,
                      nonconv_return_nan, Var::verbosity(ws));
}

/*! DOIT convergence test (least squares).

As *doit_conv_flagAbsBT* but applies a least squares convergence
test between two successive iteration fields.

Warning: This method is not recommended because this kind of
convergence test is not sufficiently strict, so that the
DOIT result might be wrong.

@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace
@param[in] epsilon - Limits for convergence. A vector with length matching
*stokes_dim* with unit [K].
@param[in] max_iterations - Maximum number of iterations allowed to reach
convergencelimit. (default: 100)
@param[in] nonconv_return_nan - Flag whether to accept result at max_iterations
(0=default)or whether to return NaNs in case of non-convergence atmax_iterations
(default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void doit_conv_flagLsq(Workspace& ws, const Vector& epsilon,
                       const Index& max_iterations = 100,
                       const Index& nonconv_return_nan = 0) {
  doit_conv_flagLsq(Var::doit_conv_flag(ws), Var::doit_iteration_counter(ws),
                    Var::cloudbox_field_mono(ws),
                    Var::cloudbox_field_mono_old(ws), Var::f_grid(ws),
                    Var::f_index(ws), epsilon, max_iterations,
                    nonconv_return_nan, Var::verbosity(ws));
}

/*! Calculates the scattering integral field in the DOIT module.

The scattering integral field is generated by integrating
the product of phase matrix and Stokes vector over all incident
angles. For more information please refer to AUG.

@author Sreerekha T.R.
@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void doit_scat_fieldCalc(Workspace& ws) {
  doit_scat_fieldCalc(ws, Var::doit_scat_field(ws), Var::pha_mat_spt_agenda(ws),
                      Var::cloudbox_field_mono(ws), Var::pnd_field(ws),
                      Var::t_field(ws), Var::atmosphere_dim(ws),
                      Var::cloudbox_limits(ws), Var::za_grid(ws),
                      Var::aa_grid(ws), Var::doit_za_grid_size(ws),
                      Var::pha_mat_doit(ws), Var::verbosity(ws));
}

/*! Calculates the scattering integral field in the DOIT module (limb).

The scattering integral field is the field generated by integrating
the product of phase matrix and the Stokes vector over all incident
angles.

For limb simulations it makes sense to use different
zenith angle grids for the scattering integral part and the RT part,
because the latter part requires a much finer resolution near
90 degrees. Taking an optimized grid for the RT part and an equidistant
grid for the scattering integral part saves very much CPU time.
This method uses the equidistant za_grid defined in
*DOAngularGridsSet* and it should always be used for limb
simulations.

For more information please refer to AUG.

@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void doit_scat_fieldCalcLimb(Workspace& ws) {
  doit_scat_fieldCalcLimb(
      ws, Var::doit_scat_field(ws), Var::pha_mat_spt_agenda(ws),
      Var::cloudbox_field_mono(ws), Var::pnd_field(ws), Var::t_field(ws),
      Var::atmosphere_dim(ws), Var::cloudbox_limits(ws), Var::za_grid(ws),
      Var::aa_grid(ws), Var::doit_za_grid_size(ws), Var::doit_za_interp(ws),
      Var::pha_mat_doit(ws), Var::verbosity(ws));
}

/*! Zenith angle grid optimization for scattering calculation.

This method optimizes the zenith angle grid. As input it requires
a radiation field (*cloudbox_field*) which is calculated on a very
fine zenith angle grid (*za_grid*). Based on this field
zenith angle grid points are selected, such that the maximum
difference between the radiation field represented on the very
fine zenith angle grid and the radiation field represented on the
optimized grid (*doit_za_grid_opt*) is less than the accuracy
(*acc*). Between the grid points the radiation field is interpolated
linearly or polynomially depending on *doit_za_interp*.

Note: The method works only for a 1D atmosphere and for one
frequency.

@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace
@param[in] acc - Accuracy to achieve [%].

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void doit_za_grid_optCalc(Workspace& ws, const Numeric& acc) {
  doit_za_grid_optCalc(Var::doit_za_grid_opt(ws), Var::cloudbox_field_mono(ws),
                       Var::za_grid(ws), Var::doit_za_interp(ws), acc,
                       Var::verbosity(ws));
}

/*! Define interpolation method for zenith angle dimension.

You can use this method to choose the interpolation method for
interpolations in the zenith angle dimension.

@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace
@param[in] interp_method - Interpolation method ("linear" or "polynomial").
(default: "linear")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void doit_za_interpSet(Workspace& ws, const String& interp_method = "linear") {
  doit_za_interpSet(Var::doit_za_interp(ws), Var::atmosphere_dim(ws),
                    interp_method, Var::verbosity(ws));
}

/*! Add gas absorption to all diagonal elements of extinction matrix.

The task of this method is to sum up the gas absorption of the
different gas species and add the result to the extinction matrix.

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ext_matAddGas(Workspace& ws) {
  ext_matAddGas(Var::ext_mat(ws), Var::propmat_clearsky(ws),
                Var::verbosity(ws));
}

/*! Sets *f_grid* to a grid relative to *abs_lines_per_species*

Each line will have *abs_lines_per_species* will have a grid
of *num_freqs* grid points in [f0+*delta_f_low*, f0+*delta_f_upp*],
where f0 is the line center.

Before leaving the function, *f_grid* is sorted.

Note that this method could generate significantly large *f_grid*
if used carelessly

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] delta_f_low - Lower range of delta f (default: -5e6)
@param[in] delta_f_upp - Upper range of delta f (default: 5e6)
@param[in] num_freqs - Number of frequencies

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void f_gridFromAbsorptionLines(Workspace& ws, const Index& num_freqs,
                               const Numeric& delta_f_low = -5e6,
                               const Numeric& delta_f_upp = 5e6) {
  f_gridFromAbsorptionLines(Var::f_grid(ws), Var::abs_lines_per_species(ws),
                            delta_f_low, delta_f_upp, num_freqs,
                            Var::verbosity(ws));
}

/*! Sets *f_grid* to the frequency grid of *abs_lookup*.

Must be called between importing/creating raw absorption table and
call of *abs_lookupAdapt*.

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void f_gridFromGasAbsLookup(Workspace& ws) {
  f_gridFromGasAbsLookup(Var::f_grid(ws), Var::abs_lookup(ws),
                         Var::verbosity(ws));
}

/*! Automatically calculate f_grid to match the sensor.

This method is handy if you are simulating an AMSU-type instrument,
consisting of a few discrete channels. The case that channels touch,
as for MHS, is handled correctly. But the case that channels overlap
is not (yet) handled and results in an error message.

The method calculates *f_grid* to match the instrument, as given by
the local oscillator frequencies *lo_multi*, the backend
frequencies *f_backend_multi*, and the backend channel
responses *backend_channel_response_multi*.

You have to specify the desired spacing in the keyword *spacing*,
which has a default value of 100 MHz. (The actual value is 0.1e9,
since our unit is Hz.)

The produced grid will not have exactly the requested spacing, but
will not be coarser than requested. The algorithm starts with the band
edges, then adds additional points until the spacing is at least as
fine as requested.

There is a similar method for HIRS-type instruments,
see *f_gridFromSensorHIRS*.

@author Stefan Buehler, Mathias Milz

@param[in,out] Workspace ws - An ARTS workspace
@param[in] spacing - Desired grid spacing in Hz. (default: .1e9)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void f_gridFromSensorAMSU(Workspace& ws, const Numeric& spacing = .1e9) {
  f_gridFromSensorAMSU(
      Var::f_grid(ws), Var::lo_multi(ws), Var::f_backend_multi(ws),
      Var::backend_channel_response_multi(ws), spacing, Var::verbosity(ws));
}

/*! Automatcially calculate f_grid to match the sensor.
This function is based on 'f_gridFromSensorAMSU'

The method calculates *f_grid* to match the instrument, as given by
the backend frequencies *f_backend*, and the backend channel
responses *backend_channel_response*.

You have to specify the desired spacing in the keyword *spacing*,
which has a default value of 100 MHz. (The actual value is 0.1e9,
since our unit is Hz.)
The produced grid will not have exactly the requested spacing, but
it will not be coarser than requested. The algorithm starts with the band
edges, then adds additional points until the spacing is at least as
fine as requested.

@author Oscar Isoz

@param[in,out] Workspace ws - An ARTS workspace
@param[in] spacing - Desired grid spacing in Hz. (default: .1e9)
@param[in] verbosityVect - Bandwidth adjusted spacing (default: {})

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void f_gridFromSensorAMSUgeneric(Workspace& ws, const Numeric& spacing = .1e9,
                                 const Vector& verbosityVect = {}) {
  f_gridFromSensorAMSUgeneric(Var::f_grid(ws), Var::f_backend_multi(ws),
                              Var::backend_channel_response_multi(ws), spacing,
                              verbosityVect, Var::verbosity(ws));
}

/*! Automatically calculate f_grid to match the sensor.

This method is handy if you are simulating a HIRS-type instrument,
consisting of a few discrete channels.

It calculates f_grid to match the instrument, as given by the nominal
band frequencies *f_backend* and the spectral channel response
functions given by *backend_channel_response*.

You have to specify the desired spacing in the keyword *spacing*, which
has a default value of 5e8 Hz.

The produced grid will not have exactly the requested spacing, but
will not be coarser than requested. The algorithm starts with the band
edges, then adds additional points until the spacing is at least as
fine as requested.

There is a similar method for AMSU-type instruments, see
*f_gridFromSensorAMSU*.

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace
@param[in] spacing - Desired grid spacing in Hz. (default: 5e8)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void f_gridFromSensorHIRS(Workspace& ws, const Numeric& spacing = 5e8) {
  f_gridFromSensorHIRS(Var::f_grid(ws), Var::f_backend(ws),
                       Var::backend_channel_response(ws), spacing,
                       Var::verbosity(ws));
}

/*! Sets *f_grid* and associated variables match MetMM settings.

The method calculates *f_grid* to match the specifications of a
*met_mm_backend* table and method arguments.

You have to specify the desired spacing using the keyword *freq_spacing*.
You can pass a *Vector* with one element to apply the same spacing to all
channels or pass a spacing value for each channel separately.

Optionally, *freq_number* can be set to specify the mininum number of
frequencies per passband for each channel. The frequencies are placed
equally spaced in each passband. The minimum spacing resulting from
*freq_number* and *freq_spacing* will be used for the calculation. To
explicitly use *freq_spacing* for a channel, *freq_number* can be set
to -1 for this channel.

The number of elements in *freq_number* can either be the number of
channels or 1. If only one element is given, this number is used for
all channels. If *freq_number* is 1 and *freq_spacing* is wider than
the bandwidth of the channel, one frequency is placed in the middle of
each passband.

Frequencies that would be closer than *freq_merge_threshold* in the
generated *f_grid* are merged together. This value should be left at
the default value. This is only meant to compensate for numerical
inaccuracies in the frequency calculation to merge frequency that are
supposed to be identical.

@author Oliver Lemke
@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] freq_spacing - Desired grid spacing in Hz. (default: {.1e9})
@param[in] freq_number - Number of frequencies per passband for each channel.
(default: {-1})
@param[in] freq_merge_threshold - Merge frequencies that are closer than this
value in Hz. (default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void f_gridMetMM(Workspace& ws, const Vector& freq_spacing = {.1e9},
                 const ArrayOfIndex& freq_number = {-1},
                 const Numeric& freq_merge_threshold = 1) {
  f_gridMetMM(Var::f_grid(ws), Var::f_backend(ws),
              Var::channel2fgrid_indexes(ws), Var::channel2fgrid_weights(ws),
              Var::met_mm_backend(ws), freq_spacing, freq_number,
              freq_merge_threshold, Var::verbosity(ws));
}

/*! Gravity at zero altitude on Earth.

Sets *g0* for the given latitude using a standard parameterisation.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void g0Earth(Workspace& ws) {
  g0Earth(Var::g0(ws), Var::lat(ws), Var::verbosity(ws));
}

/*! Gravity at zero altitude on Io.

Numeric from Wikipedia.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void g0Io(Workspace& ws) { g0Io(Var::g0(ws), Var::verbosity(ws)); }

/*! Gravity at zero altitude on Jupiter.

Sets *g0*  to mean equatorial gravity on Jupiter. Value provided by
MPS under ESA-planetary study (TN1).

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void g0Jupiter(Workspace& ws) { g0Jupiter(Var::g0(ws), Var::verbosity(ws)); }

/*! Gravity at zero altitude on Mars.

Sets *g0*  to mean equatorial gravity on Mars. Value provided by
MPS under ESA-planetary study (TN1).

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void g0Mars(Workspace& ws) { g0Mars(Var::g0(ws), Var::verbosity(ws)); }

/*! Gravity at zero altitude on Venus.

Sets *g0*  to mean equatorial gravity on Venus. Value from Ahrens
(1995), provided by MPS under ESA-planetary study (TN1).

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void g0Venus(Workspace& ws) { g0Venus(Var::g0(ws), Var::verbosity(ws)); }

/*! Sets geo-position based on *ppath*.

The geo-position is set to the position of the last point
of the present propagation path. This will be the surface,
top-of-the atmosphere or cloudbox position, depending of
observation geometry and if the cloudbox is active.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void geo_posEndOfPpath(Workspace& ws) {
  geo_posEndOfPpath(Var::geo_pos(ws), Var::ppath(ws), Var::verbosity(ws));
}

/*! Sets geo-position based on *ppath*.

The geo-position is set to the position of the last point
of the present propagation path having the lowest altitude.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void geo_posLowestAltitudeOfPpath(Workspace& ws) {
  geo_posLowestAltitudeOfPpath(Var::geo_pos(ws), Var::ppath(ws),
                               Var::verbosity(ws));
}

/*! Sets geo-position based on *ppath*.

The geo-position is set to the position where the propagation
path passes the reference altitude. If this altitude is passes
more than once, the passing closest to the sensor is selected.
If the reference altitude is not passed at all, *geo_pos* is
set to NaN.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] z_ref - Reference altitude.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void geo_posWherePpathPassesZref(Workspace& ws, const Numeric& z_ref) {
  geo_posWherePpathPassesZref(Var::geo_pos(ws), Var::ppath(ws), z_ref,
                              Var::verbosity(ws));
}

/*! Calculates heating rates. It assumes that the heating rates
depend only on the vertical derivation of the net flux.
The net flux is the sum of the irradiance field in upward
 direction and the irradiance field in downward direction

@author Manfred Brath

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void heating_ratesFromIrradiance(Workspace& ws) {
  heating_ratesFromIrradiance(
      Var::heating_rates(ws), Var::p_grid(ws), Var::irradiance_field(ws),
      Var::specific_heat_capacity(ws), Var::g0(ws), Var::verbosity(ws));
}

/*! Calculate the irradiance also known as flux density from the
*radiance_field* . by integrating over the angular grids according to the grids
set by *AngularGridsSetFluxCalc* See *AngularGridsSetFluxCalc to set *za_grid,
aa_grid, and za_grid_weights*

@author Manfred Brath

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void irradiance_fieldFromRadiance(Workspace& ws) {
  irradiance_fieldFromRadiance(
      Var::irradiance_field(ws), Var::radiance_field(ws), Var::za_grid(ws),
      Var::aa_grid(ws), Var::za_grid_weights(ws), Var::verbosity(ws));
}

/*! Initialize isotopologue ratios with default values from built-in
species data.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void isotopologue_ratiosInitFromBuiltin(Workspace& ws) {
  isotopologue_ratiosInitFromBuiltin(Var::isotopologue_ratios(ws),
                                     Var::verbosity(ws));
}

/*! Simulation of radar/lidar, restricted to single scattering.

The WSM treats e.g. radar measurements of cloud and precipitation,
on the condition that multiple scattering can be ignored. Beside
the direct backsacttering, the two-way attenuation by gases and
particles is considered. Surface scattering is ignored.

The method could potentially be used for lidars, but multiple
scattering poses here a must stronger constrain for the range of
applications.

The method can be used with *iyCalc*, but not with *yCalc*. In the
later case, use instead *yActive*.

The method returns the backscattering for each point of *ppath*.
Several frequencies can be treated in parallel. The size of *iy*
is [ nf*np, stokes_dim ], where nf is the length of *f_grid* and
np is the number of path points. The data are stored in blocks
of [ np, stokes_dim ]. That is, all the results for the first
frequency occupy the np first rows of *iy* etc.

The polarisation state of the transmitted pulse is taken from
*iy_transmitter_agenda*. If the radar transmits several polarisations
at the same frequency, you need to handle this by using two frequencies
in *f_grid*, but these can be almost identical.

This method does not consider *iy_unit*. Unit changes are insted applied
in *yActive. The output of this method matches the option "1".

Transmission is handled in a slightly simplified manner for efficiency
reasons. First of all, the transmission matrix is assumed to be the same
in both directions between the sensor and the point of back-scattering.
This should in general be true, but exceptions could exist. The extinction
due to particles can also be scaled, which could be of interest when e.g.
characterising inversions.

Further, for Jacobian calculations the default is to assume that the
transmission is unaffected by the retrieval quantities. This is done
to save computational time, and should be a valid approximation for the
single-scattering conditions. Set *trans_in_jacobian* to 1 to obtain
the more accurate Jacobian.

Some auxiliary radiative transfer quantities can be obtained. Auxiliary
quantities are selected by *iy_aux_vars* and returned by *iy_aux*.
Valid choices for auxiliary data are:
 "Radiative background": Index value flagging the radiative
    background. The following coding is used: 0=space, 1=surface
    and 2=cloudbox.
 "Backscattering": The unattenuated backscattering. That is, as
    *iy* but with no attenuated applied. Here all columns are filled.
 "Optical depth": Scalar, total and two-way, optical depth between
    sensor and each point of the propagation path. Calculated based on
    the (1,1)-element of the transmission matrix (1-based indexing),
    i.e. only fully valid for scalar RT.
 "Particle extinction": As "Optical depth", but only with particle
    attenuation included. That is, gas absorption is ignored.
If nothing else is stated, only the first column of *iy_aux* is filled,
i.e. the column matching Stokes element I, while remaing columns are
are filled with zeros.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] trans_in_jacobian - Flag determining if change in transmission is
considered in calculation of the Jacobian or not. (default: 0)
@param[in] pext_scaling - Particle extinction is scaled with this value. A value
inside [0,2]. Set it to 0 if you want to remove particle extinction totally.
(default: 1)
@param[in] t_interp_order - Interpolation order of temperature for scattering
data (so far only applied in phase matrix, not in extinction and absorption.
(default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void iyActiveSingleScat(Workspace& ws, const Index& trans_in_jacobian = 0,
                        const Numeric& pext_scaling = 1,
                        const Index& t_interp_order = 1) {
  iyActiveSingleScat(
      ws, Var::iy(ws), Var::iy_aux(ws), Var::diy_dx(ws), Var::ppvar_p(ws),
      Var::ppvar_t(ws), Var::ppvar_nlte(ws), Var::ppvar_vmr(ws),
      Var::ppvar_wind(ws), Var::ppvar_mag(ws), Var::ppvar_pnd(ws),
      Var::ppvar_f(ws), Var::ppvar_trans_cumulat(ws), Var::stokes_dim(ws),
      Var::f_grid(ws), Var::atmosphere_dim(ws), Var::p_grid(ws),
      Var::t_field(ws), Var::nlte_field(ws), Var::vmr_field(ws),
      Var::abs_species(ws), Var::wind_u_field(ws), Var::wind_v_field(ws),
      Var::wind_w_field(ws), Var::mag_u_field(ws), Var::mag_v_field(ws),
      Var::mag_w_field(ws), Var::cloudbox_on(ws), Var::cloudbox_limits(ws),
      Var::pnd_field(ws), Var::dpnd_field_dx(ws), Var::scat_species(ws),
      Var::scat_data(ws), Var::scat_data_checked(ws), Var::iy_aux_vars(ws),
      Var::jacobian_do(ws), Var::jacobian_quantities(ws), Var::ppath(ws),
      Var::propmat_clearsky_agenda(ws), Var::water_p_eq_agenda(ws),
      Var::iy_transmitter_agenda(ws), Var::iy_agenda_call1(ws),
      Var::iy_transmission(ws), Var::rte_alonglos_v(ws), trans_in_jacobian,
      pext_scaling, t_interp_order, Var::verbosity(ws));
}

/*! TESTING

@author Patrick Eriksson
@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] trans_in_jacobian - Flag determining if change in transmission is
considered in calculation of the Jacobian or not. (default: 0)
@param[in] pext_scaling - Particle extinction is scaled with this value. A value
inside [0,2]. Set it to 0 if you want to remove particle extinction totally.
(default: 1)
@param[in] t_interp_order - Interpolation order of temperature for scattering
data (so far only applied in phase matrix, not in extinction and absorption.
(default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void iyActiveSingleScat2(Workspace& ws, const Index& trans_in_jacobian = 0,
                         const Numeric& pext_scaling = 1,
                         const Index& t_interp_order = 1) {
  iyActiveSingleScat2(
      ws, Var::iy(ws), Var::iy_aux(ws), Var::diy_dx(ws), Var::ppvar_p(ws),
      Var::ppvar_t(ws), Var::ppvar_nlte(ws), Var::ppvar_vmr(ws),
      Var::ppvar_wind(ws), Var::ppvar_mag(ws), Var::ppvar_pnd(ws),
      Var::ppvar_f(ws), Var::ppvar_trans_cumulat(ws), Var::stokes_dim(ws),
      Var::f_grid(ws), Var::atmosphere_dim(ws), Var::p_grid(ws),
      Var::t_field(ws), Var::nlte_field(ws), Var::vmr_field(ws),
      Var::abs_species(ws), Var::wind_u_field(ws), Var::wind_v_field(ws),
      Var::wind_w_field(ws), Var::mag_u_field(ws), Var::mag_v_field(ws),
      Var::mag_w_field(ws), Var::cloudbox_on(ws), Var::cloudbox_limits(ws),
      Var::pnd_field(ws), Var::dpnd_field_dx(ws), Var::scat_species(ws),
      Var::scat_data(ws), Var::scat_data_checked(ws), Var::iy_aux_vars(ws),
      Var::jacobian_do(ws), Var::jacobian_quantities(ws), Var::ppath(ws),
      Var::propmat_clearsky_agenda(ws), Var::water_p_eq_agenda(ws),
      Var::iy_transmitter_agenda(ws), Var::iy_agenda_call1(ws),
      Var::iy_transmission(ws), Var::rte_alonglos_v(ws), trans_in_jacobian,
      pext_scaling, t_interp_order, Var::verbosity(ws));
}

/*! Conversion of *iy* to other spectral units.

The method allows a change of unit, as a post-processing step,
ignoring the n2-law of radiance.

The conversion made inside *iyEmissionStandard* is mimiced,
see that method for constraints and selection of output units.
Restricted to that the n2-law can be ignored. This assumption
is valid if the sensor is placed in space, or if the refractive
index only deviates slightly from unity.

It is stressed that there is no automatic check that the method is
applied correctly, it is up to the user to ensure that the input
data are suitable for the conversion.

Beside *iy*, these auxilary quantities are modified:
    "iy", "Error" and "Error (uncorrelated)"

Please note that *diy_dx* is not handled.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void iyApplyUnit(Workspace& ws) {
  iyApplyUnit(Var::iy(ws), Var::iy_aux(ws), Var::stokes_dim(ws),
              Var::f_grid(ws), Var::iy_aux_vars(ws), Var::iy_unit(ws),
              Var::verbosity(ws));
}

/*! A single monochromatic pencil beam calculation.

Performs monochromatic radiative transfer calculations for the
specified position (*rte_pos*) and line-of-sight (*rte_pos*).
See *iy* and associated variables for format of output.

Please note that Jacobian type calculations not are supported.
For this use *yCalc*.

No sensor characteristics are applied. These are most easily
incorporated by using *yCalc*

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void iyCalc(Workspace& ws) {
  iyCalc(ws, Var::iy(ws), Var::iy_aux(ws), Var::ppath(ws),
         Var::atmgeom_checked(ws), Var::atmfields_checked(ws),
         Var::iy_aux_vars(ws), Var::iy_id(ws), Var::cloudbox_on(ws),
         Var::cloudbox_checked(ws), Var::scat_data_checked(ws), Var::f_grid(ws),
         Var::nlte_field(ws), Var::rte_pos(ws), Var::rte_los(ws),
         Var::rte_pos2(ws), Var::iy_unit(ws), Var::iy_main_agenda(ws),
         Var::verbosity(ws));
}

/*! Standard method for radiative transfer calculations with emission.

Designed to be part of *iy_main_agenda*. That is, only valid
outside the cloudbox (no scattering). For details se the user guide.

The possible choices for *iy_unit* are
 "1"             : No conversion, i.e. [W/(m^2 Hz sr)] (radiance per
                     frequency unit).
 "RJBT"          : Conversion to Rayleigh-Jean brightness
                     temperature.
 "PlanckBT"      : Conversion to Planck brightness temperature.
 "W/(m^2 m sr)"  : Conversion to [W/(m^2 m sr)] (radiance per
                     wavelength unit).
 "W/(m^2 m-1 sr)": Conversion to [W/(m^2 m-1 sr)] (radiance per
                     wavenumber unit).
Expressions applied and considerations for the unit conversion of
radiances are discussed in Sec. 5.7 of the ARTS-2.0 article.

*iy_unit* is only applied if *iy_agenda_call1* is 1. This means that
no unit ocnversion is applied for internal iterative calls.

Some auxiliary radiative transfer quantities can be obtained. Auxiliary
quantities are selected by *iy_aux_vars* and returned by *iy_aux*.
Valid choices for auxiliary data are:
 "Radiative background": Index value flagging the radiative
    background. The following coding is used: 0=space, 1=surface
    and 2=cloudbox.
 "Optical depth": Scalar optical depth between the observation point
    and the end of the present propagation path. Calculated based on
    the (1,1)-element of the transmission matrix (1-based indexing),
    i.e. only fully valid for scalar RT.
If nothing else is stated, only the first column of *iy_aux* is filled,
i.e. the column matching Stokes element I, while remaing columns are
are filled with zeros.

@author Patrick Eriksson
@author Richard Larsson
@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void iyEmissionStandard(Workspace& ws) {
  iyEmissionStandard(
      ws, Var::iy(ws), Var::iy_aux(ws), Var::diy_dx(ws), Var::ppvar_p(ws),
      Var::ppvar_t(ws), Var::ppvar_nlte(ws), Var::ppvar_vmr(ws),
      Var::ppvar_wind(ws), Var::ppvar_mag(ws), Var::ppvar_f(ws),
      Var::ppvar_iy(ws), Var::ppvar_trans_cumulat(ws),
      Var::ppvar_trans_partial(ws), Var::iy_id(ws), Var::stokes_dim(ws),
      Var::f_grid(ws), Var::atmosphere_dim(ws), Var::p_grid(ws),
      Var::t_field(ws), Var::nlte_field(ws), Var::vmr_field(ws),
      Var::abs_species(ws), Var::wind_u_field(ws), Var::wind_v_field(ws),
      Var::wind_w_field(ws), Var::mag_u_field(ws), Var::mag_v_field(ws),
      Var::mag_w_field(ws), Var::cloudbox_on(ws), Var::iy_unit(ws),
      Var::iy_aux_vars(ws), Var::jacobian_do(ws), Var::jacobian_quantities(ws),
      Var::ppath(ws), Var::rte_pos2(ws), Var::propmat_clearsky_agenda(ws),
      Var::water_p_eq_agenda(ws), Var::iy_main_agenda(ws),
      Var::iy_space_agenda(ws), Var::iy_surface_agenda(ws),
      Var::iy_cloudbox_agenda(ws), Var::iy_agenda_call1(ws),
      Var::iy_transmission(ws), Var::rte_alonglos_v(ws),
      Var::surface_props_data(ws), Var::verbosity(ws));
}

/*! DEPRECATED! Should go away soon
Sequential version of *iyEmissionStandard*

For documentation see *iyEmissionStandard*.

@author Patrick Eriksson
@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void iyEmissionStandardSequential(Workspace& ws) {
  iyEmissionStandardSequential(
      ws, Var::iy(ws), Var::iy_aux(ws), Var::diy_dx(ws), Var::ppvar_p(ws),
      Var::ppvar_t(ws), Var::ppvar_nlte(ws), Var::ppvar_vmr(ws),
      Var::ppvar_wind(ws), Var::ppvar_mag(ws), Var::ppvar_f(ws),
      Var::ppvar_iy(ws), Var::ppvar_trans_cumulat(ws),
      Var::ppvar_trans_partial(ws), Var::iy_id(ws), Var::stokes_dim(ws),
      Var::f_grid(ws), Var::atmosphere_dim(ws), Var::p_grid(ws),
      Var::t_field(ws), Var::nlte_field(ws), Var::vmr_field(ws),
      Var::abs_species(ws), Var::wind_u_field(ws), Var::wind_v_field(ws),
      Var::wind_w_field(ws), Var::mag_u_field(ws), Var::mag_v_field(ws),
      Var::mag_w_field(ws), Var::cloudbox_on(ws), Var::iy_unit(ws),
      Var::iy_aux_vars(ws), Var::jacobian_do(ws), Var::jacobian_quantities(ws),
      Var::ppath(ws), Var::rte_pos2(ws), Var::propmat_clearsky_agenda(ws),
      Var::water_p_eq_agenda(ws), Var::iy_main_agenda(ws),
      Var::iy_space_agenda(ws), Var::iy_surface_agenda(ws),
      Var::iy_cloudbox_agenda(ws), Var::iy_agenda_call1(ws),
      Var::iy_transmission(ws), Var::rte_alonglos_v(ws),
      Var::surface_props_data(ws), Var::verbosity(ws));
}

/*! So far just for testing.

@author Patrick Eriksson
@author Jana Mendrok
@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] Naa_grid - Number of azimuth angles to consider in scattering source
term integral. (default: 19)
@param[in] t_interp_order - Interpolation order of temperature for scattering
data (so far only applied in phase matrix, not in extinction and absorption.
(default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void iyHybrid(Workspace& ws, const Index& Naa_grid = 19,
              const Index& t_interp_order = 1) {
  iyHybrid(ws, Var::iy(ws), Var::iy_aux(ws), Var::diy_dx(ws), Var::ppvar_p(ws),
           Var::ppvar_t(ws), Var::ppvar_nlte(ws), Var::ppvar_vmr(ws),
           Var::ppvar_wind(ws), Var::ppvar_mag(ws), Var::ppvar_pnd(ws),
           Var::ppvar_f(ws), Var::ppvar_iy(ws), Var::ppvar_trans_cumulat(ws),
           Var::iy_id(ws), Var::stokes_dim(ws), Var::f_grid(ws),
           Var::atmosphere_dim(ws), Var::p_grid(ws), Var::t_field(ws),
           Var::nlte_field(ws), Var::vmr_field(ws), Var::abs_species(ws),
           Var::wind_u_field(ws), Var::wind_v_field(ws), Var::wind_w_field(ws),
           Var::mag_u_field(ws), Var::mag_v_field(ws), Var::mag_w_field(ws),
           Var::cloudbox_on(ws), Var::cloudbox_limits(ws), Var::pnd_field(ws),
           Var::dpnd_field_dx(ws), Var::scat_species(ws), Var::scat_data(ws),
           Var::iy_unit(ws), Var::iy_aux_vars(ws), Var::jacobian_do(ws),
           Var::jacobian_quantities(ws), Var::propmat_clearsky_agenda(ws),
           Var::water_p_eq_agenda(ws), Var::iy_main_agenda(ws),
           Var::iy_space_agenda(ws), Var::iy_surface_agenda(ws),
           Var::iy_cloudbox_agenda(ws), Var::iy_agenda_call1(ws),
           Var::iy_transmission(ws), Var::ppath(ws), Var::rte_pos2(ws),
           Var::rte_alonglos_v(ws), Var::surface_props_data(ws),
           Var::cloudbox_field(ws), Var::za_grid(ws), Naa_grid, t_interp_order,
           Var::verbosity(ws));
}

/*! So far just for even more testing.

@author Patrick Eriksson
@author Jana Mendrok
@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] Naa_grid - Number of azimuth angles to consider in scattering source
term integral. (default: 19)
@param[in] t_interp_order - Interpolation order of temperature for scattering
data (so far only applied in phase matrix, not in extinction and absorption.
(default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void iyHybrid2(Workspace& ws, const Index& Naa_grid = 19,
               const Index& t_interp_order = 1) {
  iyHybrid2(ws, Var::iy(ws), Var::iy_aux(ws), Var::diy_dx(ws), Var::ppvar_p(ws),
            Var::ppvar_t(ws), Var::ppvar_nlte(ws), Var::ppvar_vmr(ws),
            Var::ppvar_wind(ws), Var::ppvar_mag(ws), Var::ppvar_pnd(ws),
            Var::ppvar_f(ws), Var::ppvar_iy(ws), Var::ppvar_trans_cumulat(ws),
            Var::iy_id(ws), Var::stokes_dim(ws), Var::f_grid(ws),
            Var::atmosphere_dim(ws), Var::p_grid(ws), Var::t_field(ws),
            Var::nlte_field(ws), Var::vmr_field(ws), Var::abs_species(ws),
            Var::wind_u_field(ws), Var::wind_v_field(ws), Var::wind_w_field(ws),
            Var::mag_u_field(ws), Var::mag_v_field(ws), Var::mag_w_field(ws),
            Var::cloudbox_on(ws), Var::cloudbox_limits(ws), Var::pnd_field(ws),
            Var::dpnd_field_dx(ws), Var::scat_species(ws), Var::scat_data(ws),
            Var::iy_unit(ws), Var::iy_aux_vars(ws), Var::jacobian_do(ws),
            Var::jacobian_quantities(ws), Var::propmat_clearsky_agenda(ws),
            Var::water_p_eq_agenda(ws), Var::iy_main_agenda(ws),
            Var::iy_space_agenda(ws), Var::iy_surface_agenda(ws),
            Var::iy_cloudbox_agenda(ws), Var::iy_agenda_call1(ws),
            Var::iy_transmission(ws), Var::ppath(ws), Var::rte_pos2(ws),
            Var::rte_alonglos_v(ws), Var::surface_props_data(ws),
            Var::cloudbox_field(ws), Var::za_grid(ws), Naa_grid, t_interp_order,
            Var::verbosity(ws));
}

/*! In development ....

Describe how *atm_fields_compact* is filled.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] return_atm1d - Flag to trigger that *atm_fields_compact* is filled.
(default: 0)
@param[in] skip_vmr - Flag to not include vmr data in *atm_fields_compact*.
(default: 0)
@param[in] skip_pnd - Flag to not include pnd data in *atm_fields_compact*.
(default: 0)
@param[in] return_masses - Flag to include particle category masses in
*atm_fields_compact*.Conversion is done by *particle_masses*. (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void iyIndependentBeamApproximation(Workspace& ws,
                                    const Index& return_atm1d = 0,
                                    const Index& skip_vmr = 0,
                                    const Index& skip_pnd = 0,
                                    const Index& return_masses = 0) {
  iyIndependentBeamApproximation(
      ws, Var::iy(ws), Var::iy_aux(ws), Var::ppath(ws), Var::diy_dx(ws),
      Var::atm_fields_compact(ws), Var::iy_id(ws), Var::f_grid(ws),
      Var::atmosphere_dim(ws), Var::p_grid(ws), Var::lat_grid(ws),
      Var::lon_grid(ws), Var::lat_true(ws), Var::lon_true(ws), Var::t_field(ws),
      Var::z_field(ws), Var::vmr_field(ws), Var::nlte_field(ws),
      Var::wind_u_field(ws), Var::wind_v_field(ws), Var::wind_w_field(ws),
      Var::mag_u_field(ws), Var::mag_v_field(ws), Var::mag_w_field(ws),
      Var::cloudbox_on(ws), Var::cloudbox_limits(ws), Var::pnd_field(ws),
      Var::particle_masses(ws), Var::ppath_agenda(ws), Var::ppath_lmax(ws),
      Var::ppath_lraytrace(ws), Var::iy_agenda_call1(ws), Var::iy_unit(ws),
      Var::iy_transmission(ws), Var::rte_pos(ws), Var::rte_los(ws),
      Var::rte_pos2(ws), Var::jacobian_do(ws), Var::iy_aux_vars(ws),
      Var::iy_independent_beam_approx_agenda(ws), return_atm1d, skip_vmr,
      skip_pnd, return_masses, Var::verbosity(ws));
}

/*! Interpolates the intensity field of the cloud box.

Determines the intensity field at the position and direction
specified by *rte_pos* and *rte_los*. The position can be both
inside the cloud box or at its edge.

The interpolation in the spatial dimensions is linear.

For the zenith angle dimensions several options for controlling
the interpolation are at hand. Default is linear interpolation.
Higher order polynomial interpolation is activated by setting
*za_interp_order* to a value > 1. Default is to perform the
interpolation separately for [0,90[ and ]90,180]. To handle
90 degree or use the full range ([0,180]) as basis for the
interpolation, set *za_restrict* to 0. You can select to use
cos(za) as the independent variable (instead of za) by setting
*cos_za_interp* to 1.

For the azimuth dimension the interpolation order can be
selected, in the same manner as for zenith.

@author Claudia Emde
@author Patrick Eriksson
@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] za_interp_order - Zenith angle interpolation order. (default: 1)
@param[in] za_restrict - Flag whether to restric zenith angle interpolation to
one hemisphere. (default: 1)
@param[in] cos_za_interp - Flag whether to do zenith angle interpolation in
cosine space. (default: 0)
@param[in] za_extpolfac - Maximum allowed extrapolation range in zenith angle.
(default: 0.5)
@param[in] aa_interp_order - Azimuth angle interpolation order. (default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void iyInterpCloudboxField(Workspace& ws, const Index& za_interp_order = 1,
                           const Index& za_restrict = 1,
                           const Index& cos_za_interp = 0,
                           const Numeric& za_extpolfac = 0.5,
                           const Index& aa_interp_order = 1) {
  iyInterpCloudboxField(
      Var::iy(ws), Var::cloudbox_field(ws), Var::rtp_pos(ws), Var::rtp_los(ws),
      Var::jacobian_do(ws), Var::cloudbox_on(ws), Var::cloudbox_limits(ws),
      Var::atmosphere_dim(ws), Var::p_grid(ws), Var::lat_grid(ws),
      Var::lon_grid(ws), Var::z_field(ws), Var::z_surface(ws),
      Var::stokes_dim(ws), Var::za_grid(ws), Var::aa_grid(ws), Var::f_grid(ws),
      za_interp_order, za_restrict, cos_za_interp, za_extpolfac,
      aa_interp_order, Var::verbosity(ws));
}

/*! Radiative transfer calculations one frequency at the time.

The method loops the frequencies in *f_grid* and calls
*iy_loop_freqs_agenda* for each individual value. This method is
placed in *iy_main_agenda*, and the actual radiative transfer
 method is put in *iy_loop_freqs_agenda*.

A common justification for using the method should be to consider
dispersion. By using this method it is ensured that the propagation
path for each individual frequency is calculated.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void iyLoopFrequencies(Workspace& ws) {
  iyLoopFrequencies(ws, Var::iy(ws), Var::iy_aux(ws), Var::ppath(ws),
                    Var::diy_dx(ws), Var::iy_aux_vars(ws),
                    Var::iy_agenda_call1(ws), Var::iy_transmission(ws),
                    Var::rte_pos(ws), Var::rte_los(ws), Var::rte_pos2(ws),
                    Var::stokes_dim(ws), Var::f_grid(ws),
                    Var::iy_loop_freqs_agenda(ws), Var::verbosity(ws));
}

/*! Interface to Monte Carlo part for *iy_main_agenda*.

Basically an interface to *MCGeneral* for doing monochromatic
pencil beam calculations. This functions allows Monte Carlo (MC)
calculations for sets of frequencies and sensor pos/los in a single
run. Sensor responses can be included in the standard manner
(through *yCalc*).

This function does not apply the MC approach when it comes
to sensor properties. These properties are not considered when
tracking photons, which is done in *MCGeneral* (but then only for
the antenna pattern).

Output unit options  (*iy_unit*) exactly as for *MCGeneral*.

The MC calculation errors are all assumed be uncorrelated and each
have a normal distribution. These properties are of relevance when
weighting the errors with the sensor repsonse matrix. The seed is
reset for each call of *MCGeneral* to obtain uncorrelated errors.

MC control arguments (mc_std_err, mc_max_time, mc_min_iter, mc_max_iter
mc_taustep_limit) as for *MCGeneral*. The arguments are applied
for each monochromatic pencil beam calculation individually.
As for *MCGeneral*, the value of *mc_error* shall be adopted to
*iy_unit*.

The following auxiliary data can be obtained:
  "Error (uncorrelated)": Calculation error. Size: [nf,ns,1,1].
    (The later part of the text string is required. It is used as
    a flag to yCalc for how to apply the sensor data.)
where
  nf: Number of frequencies.
  ns: Number of Stokes elements.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] t_interp_order - Interpolation order of temperature for scattering
data (so far only applied in phase matrix, not in extinction and absorption.
(default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void iyMC(Workspace& ws, const Index& t_interp_order = 1) {
  iyMC(ws, Var::iy(ws), Var::iy_aux(ws), Var::diy_dx(ws),
       Var::iy_agenda_call1(ws), Var::iy_transmission(ws), Var::rte_pos(ws),
       Var::rte_los(ws), Var::iy_aux_vars(ws), Var::jacobian_do(ws),
       Var::atmosphere_dim(ws), Var::p_grid(ws), Var::lat_grid(ws),
       Var::lon_grid(ws), Var::z_field(ws), Var::t_field(ws),
       Var::vmr_field(ws), Var::refellipsoid(ws), Var::z_surface(ws),
       Var::cloudbox_on(ws), Var::cloudbox_limits(ws), Var::stokes_dim(ws),
       Var::f_grid(ws), Var::scat_data(ws), Var::iy_space_agenda(ws),
       Var::surface_rtprop_agenda(ws), Var::propmat_clearsky_agenda(ws),
       Var::ppath_step_agenda(ws), Var::ppath_lmax(ws),
       Var::ppath_lraytrace(ws), Var::pnd_field(ws), Var::iy_unit(ws),
       Var::mc_std_err(ws), Var::mc_max_time(ws), Var::mc_max_iter(ws),
       Var::mc_min_iter(ws), Var::mc_taustep_limit(ws), t_interp_order,
       Var::verbosity(ws));
}

/*! Change of main output variable.

With this method you can replace the content of *iy* with one of
the auxiliary variables. The selected variable (by *aux_var*) must
be part of *iy_aux_vars*. The corresponding data from *iy_aux* are
copied to form a new *iy* (*iy_aux* is left unchanged). Elements of
*iy* correponding to Stokes elements not covered by the auxiliary
variable are just set to zero.

Jacobian variables are not handled.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] aux_var - Auxiliary variable to insert as *iy*.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void iyReplaceFromAux(Workspace& ws, const String& aux_var) {
  iyReplaceFromAux(Var::iy(ws), Var::iy_aux(ws), Var::iy_aux_vars(ws),
                   Var::jacobian_do(ws), aux_var, Var::verbosity(ws));
}

/*! Switch between the elements of *iy_surface_agenda_array*.

This method simply calls the agenda matching *surface_type* and
returns the results. That is, the agenda in *iy_surface_agenda_array*
with index *surface_type* (0-based) is called.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void iySurfaceCallAgendaX(Workspace& ws) {
  iySurfaceCallAgendaX(ws, Var::iy(ws), Var::diy_dx(ws), Var::iy_unit(ws),
                       Var::iy_transmission(ws), Var::iy_id(ws),
                       Var::cloudbox_on(ws), Var::jacobian_do(ws),
                       Var::f_grid(ws), Var::iy_main_agenda(ws),
                       Var::rtp_pos(ws), Var::rtp_los(ws), Var::rte_pos2(ws),
                       Var::iy_surface_agenda_array(ws), Var::surface_type(ws),
                       Var::surface_type_aux(ws), Var::verbosity(ws));
}

/*! Usage of FASTEM for emissivity and reflectivity of water surfaces.

This method allows usage of the FASTEM model inside
*iy_surface_agenda*. The aim is to use FASTEM in the exact same
way as done in RTTOV. For example, the transmittance for down-
welling radiation is considered. RTTOV os just 1D. Here 2D and 3D
are handled as the 1D case, the down-welling radiation is just
calculated for the directuon matching specular reflection.

The wind direction is given as the azimuth angle, counted
clockwise from north (i.e. an easterly wind is at 90 deg).
This matches the general definition of azimuth inside ARTS.
For 1D and 2D, the wind direction must be adjusted to match the
fact that the line-of-sight is locked to be at 0 deg (180 for 2D
in the case of a negative zenith angle). For 3D, the true wind
direction shall be used.

FASTEM is called by *FastemStandAlone*. See that WSM for further
comments on variables and limitations.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] salinity - Salinity, 0-1. That is, 3% is given as 0.03. (default:
0.035)
@param[in] wind_speed - Wind speed.
@param[in] wind_direction - Wind direction. See further above. (default: 0)
@param[in] fastem_version - The version of FASTEM to use. (default: 6)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void iySurfaceFastem(Workspace& ws, const Numeric& wind_speed,
                     const Numeric& salinity = 0.035,
                     const Numeric& wind_direction = 0,
                     const Index& fastem_version = 6) {
  iySurfaceFastem(
      ws, Var::iy(ws), Var::diy_dx(ws), Var::iy_transmission(ws),
      Var::iy_id(ws), Var::jacobian_do(ws), Var::atmosphere_dim(ws),
      Var::nlte_field(ws), Var::cloudbox_on(ws), Var::stokes_dim(ws),
      Var::f_grid(ws), Var::rtp_pos(ws), Var::rtp_los(ws), Var::rte_pos2(ws),
      Var::iy_unit(ws), Var::iy_main_agenda(ws), Var::surface_skin_t(ws),
      salinity, wind_speed, wind_direction, fastem_version, Var::verbosity(ws));
}

/*! Interface to *surface_rtprop_agenda* for *iy_surface_agenda*.

This method is designed to be part of *iy_surface_agenda*. It
determines the radiative properties of the surface by
*surface_rtprop_agenda* and calculates the downwelling radiation
by *iy_main_agenda*, and sums up the terms as described in AUG.
That is, this WSM uses the output from *surface_rtprop_agenda*
in a straightforward fashion.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void iySurfaceRtpropAgenda(Workspace& ws) {
  iySurfaceRtpropAgenda(
      ws, Var::iy(ws), Var::diy_dx(ws), Var::iy_transmission(ws),
      Var::iy_id(ws), Var::jacobian_do(ws), Var::atmosphere_dim(ws),
      Var::nlte_field(ws), Var::cloudbox_on(ws), Var::stokes_dim(ws),
      Var::f_grid(ws), Var::rtp_pos(ws), Var::rtp_los(ws), Var::rte_pos2(ws),
      Var::iy_unit(ws), Var::iy_main_agenda(ws), Var::surface_rtprop_agenda(ws),
      Var::verbosity(ws));
}

/*! Applies *surface_los*, *surface_rmatrix* and *surface_emission*.

This method is designed to be part of *iy_surface_agenda* and
should be mandatory when using methods describing the surface
radiative transfer properties by *surface_los*, *surface_rmatrix*
and *surface_emission*. The task of this method is to apply these
three WSVs to obtain the upwelling radiation from the surface.
This upwelling radiation is the sum of surface emission and
reflected downwelling radiation. The later part is calculated
by calling *iy_main_agenda*. See further AUG.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void iySurfaceRtpropCalc(Workspace& ws) {
  iySurfaceRtpropCalc(ws, Var::iy(ws), Var::diy_dx(ws), Var::surface_los(ws),
                      Var::surface_rmatrix(ws), Var::surface_emission(ws),
                      Var::dsurface_names(ws), Var::dsurface_rmatrix_dx(ws),
                      Var::dsurface_emission_dx(ws), Var::iy_transmission(ws),
                      Var::iy_id(ws), Var::jacobian_do(ws),
                      Var::jacobian_quantities(ws), Var::atmosphere_dim(ws),
                      Var::nlte_field(ws), Var::cloudbox_on(ws),
                      Var::stokes_dim(ws), Var::f_grid(ws), Var::rtp_pos(ws),
                      Var::rtp_los(ws), Var::rte_pos2(ws), Var::iy_unit(ws),
                      Var::iy_main_agenda(ws), Var::verbosity(ws));
}

/*! Standard method for handling transmission measurements.

Designed to be part of *iy_main_agenda*. Treatment of the cloudbox
is incorporated (that is, no need to define *iy_cloudbox_agenda*).

The transmitter is assumed to be placed at the end of provided *ppath*.
The transmitted signal is taken from *iy_transmitter_agenda*. This
signal is propagated along the path, considering attenuation alone.
That is, the result of the method (*iy*) is the output of
*iy_transmitter_agenda* multiplied with the transmission along the
propagation path.

As mentioned, the given *ppath* determines the position of the
transmitter. For clear-sky and no modification of *ppath*, this
means that the transitter will either be found at the surface or
at the top-of-the-atmosphere. If you want to maintain this even with
an active cloudbox, calculate *ppath* as
     ppathCalc( cloudbox_on=0 )
Without setting cloudbox_on=0, the transmitter will end up inside or
at the boundary of the cloudbox.

Some auxiliary radiative transfer quantities can be obtained. Auxiliary
quantities are selected by *iy_aux_vars* and returned by *iy_aux*.
Valid choices for auxiliary data are:
 "Radiative background": Index value flagging the radiative
    background. The following coding is used: 0=space, 1=surface
    and 2=cloudbox. The value is added to each column.
 "Optical depth": Scalar optical depth between the observation point
    and the end of the present propagation path. Calculated based on
    the (1,1)-element of the transmission matrix (1-based indexing),
    i.e. only fully valid for scalar RT. The value is added to each
    column.

@author Patrick Eriksson
@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void iyTransmissionStandard(Workspace& ws) {
  iyTransmissionStandard(
      ws, Var::iy(ws), Var::iy_aux(ws), Var::diy_dx(ws), Var::ppvar_p(ws),
      Var::ppvar_t(ws), Var::ppvar_nlte(ws), Var::ppvar_vmr(ws),
      Var::ppvar_wind(ws), Var::ppvar_mag(ws), Var::ppvar_pnd(ws),
      Var::ppvar_f(ws), Var::ppvar_iy(ws), Var::ppvar_trans_cumulat(ws),
      Var::stokes_dim(ws), Var::f_grid(ws), Var::atmosphere_dim(ws),
      Var::p_grid(ws), Var::t_field(ws), Var::nlte_field(ws),
      Var::vmr_field(ws), Var::abs_species(ws), Var::wind_u_field(ws),
      Var::wind_v_field(ws), Var::wind_w_field(ws), Var::mag_u_field(ws),
      Var::mag_v_field(ws), Var::mag_w_field(ws), Var::cloudbox_on(ws),
      Var::cloudbox_limits(ws), Var::pnd_field(ws), Var::dpnd_field_dx(ws),
      Var::scat_species(ws), Var::scat_data(ws), Var::iy_aux_vars(ws),
      Var::jacobian_do(ws), Var::jacobian_quantities(ws), Var::ppath(ws),
      Var::propmat_clearsky_agenda(ws), Var::water_p_eq_agenda(ws),
      Var::iy_transmitter_agenda(ws), Var::iy_agenda_call1(ws),
      Var::iy_transmission(ws), Var::rte_alonglos_v(ws), Var::verbosity(ws));
}

/*! Transmitted signal having multiple polarisations.

The method is intended to be part of *iy_transmitter_agenda*. It
sets *iy* to describe the transmitted signal/pulses. The polarisation
state is taken from *instrument_pol*, where *instrument_pol* must
contain an element for each frequency in *f_grid*. The transmitted
signal/pulses are set to be of unit magnitude, such as [1,1,0,0].

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void iy_transmitterMultiplePol(Workspace& ws) {
  iy_transmitterMultiplePol(Var::iy(ws), Var::stokes_dim(ws), Var::f_grid(ws),
                            Var::instrument_pol(ws), Var::verbosity(ws));
}

/*! Transmitted signal having a single polarisations.

The method is intended to be part of *iy_transmitter_agenda*. It
sets *iy* to describe the transmitted pulses/signal. The polarisation
state is taken from *instrument_pol*, where *instrument_pol* must contain
a single value. This polarisation state is applied for all
frequencies. The transmitted pulses/signals are set to be of unit
magnitude, such as [1,1,0,0].

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void iy_transmitterSinglePol(Workspace& ws) {
  iy_transmitterSinglePol(Var::iy(ws), Var::stokes_dim(ws), Var::f_grid(ws),
                          Var::instrument_pol(ws), Var::verbosity(ws));
}

/*! Includes an absorption species in the Jacobian.

For 1D or 2D calculations the latitude and/or longitude grid of
the retrieval field should set to have zero length.

These retrieval units are at hand for all gas species:
   "vmr"    : Volume mixing ratio.
   "nd"     : Number density.
   "rel"    : Relative unit (e.g. 1.1 means 10% more of the gas).

For water vapour, also these units are at hand:
   "rh"     : Relative humidity.
   "q"      : Specific humidity.

Note that *for_species_tag* is used to indicate if species tag VMR,
rather than atmospheric gas VMR is calculated. Set it to 0 and we
calculate the atmospheric gas VMR, but this only works for "analytical".

Note that the Jacobian is set to zero where volume mixing ratio equals zero.

The number of elements added to the state vector (*x*) is:
   n_g1 * n_g2 * n_g3
where n_g1, n_g2 and n_g3 are the length of GIN *g1*, *g2* and *g3*,
respectively. Here empty vectors should be considered to have a length 1.
The elements are sorted with pressure as innermost loop, followed by
latitude and longitude as outermost loop.

@author Mattias Ekstrom
@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] g1 - Pressure retrieval grid.
@param[in] g2 - Latitude retrieval grid.
@param[in] g3 - Longitude retreival grid.
@param[in] species - The species tag of the retrieval quantity.
@param[in] unit - Retrieval unit. See above. (default: "vmr")
@param[in] for_species_tag - Index-bool for acting on species tags or species.
(default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianAddAbsSpecies(Workspace& ws, const Vector& g1, const Vector& g2,
                           const Vector& g3, const String& species,
                           const String& unit = "vmr",
                           const Index& for_species_tag = 1) {
  jacobianAddAbsSpecies(ws, Var::jacobian_quantities(ws),
                        Var::jacobian_agenda(ws), Var::atmosphere_dim(ws),
                        Var::p_grid(ws), Var::lat_grid(ws), Var::lon_grid(ws),
                        g1, g2, g3, species, unit, for_species_tag,
                        Var::verbosity(ws));
}

/*! Includes a basic catalog parameter in the Jacobian. These are constant
over all layers and so only a single vector output is returned.

The only basic catalog parameters currently supported are:
   "Line Strength"
   "Line Center"

The *catalog_identity* should be able to identify one or many
lines in the catalog used for calculating the spectral absorption.
Note that partial matching for energy levels are allowed but not
recommended, as it is somewhat nonsensical to add multiple parameters

Also note *jacobianAddShapeCatalogParameter* as this allows addition
of shape parameters, e.g., pressure broadening coefficients

Each call to this function adds just a single value to *x*.

Example given the catalog_identity="O2-66 TR UP v1 0 J 1 LO v1 0 J 0",
only the O2 ground-level 119 GHz line can be accessed and only its
catalog_parameter will be accessed.  However, the more lenient
catalog_identity="O2-66 TR UP J 1 LO J 0" may be used, but then the
118 GHz line belonging to v1=1 branch will be added to the same *x*.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] catalog_identity - The catalog line matching information.
@param[in] catalog_parameter - The catalog parameter of the retrieval quantity.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianAddBasicCatalogParameter(Workspace& ws,
                                      const QuantumIdentifier& catalog_identity,
                                      const String& catalog_parameter) {
  jacobianAddBasicCatalogParameter(ws, Var::jacobian_quantities(ws),
                                   Var::jacobian_agenda(ws), catalog_identity,
                                   catalog_parameter, Var::verbosity(ws));
}

/*! See *jacobianAddBasicCatalogParameter*.

This adds a multiple of parameters for first each catalog_identity in
catalog_identities and then for each catalog_parameter in catalog_parameters
by looping calls to *jacobianAddBasicCatalogParameter* over these input

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] catalog_identities - The catalog line matching information.
@param[in] catalog_parameters - The catalog parameter of the retrieval quantity.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianAddBasicCatalogParameters(
    Workspace& ws, const ArrayOfQuantumIdentifier& catalog_identities,
    const ArrayOfString& catalog_parameters) {
  jacobianAddBasicCatalogParameters(
      ws, Var::jacobian_quantities(ws), Var::jacobian_agenda(ws),
      catalog_identities, catalog_parameters, Var::verbosity(ws));
}

/*! Includes a frequency fit of shift type in the Jacobian.

Retrieval of deviations between nominal and actual backend
frequencies can be included by this method. The assumption here is
that the deviation is a constant off-set, a shift, common for all
frequencies (and not varying between measurement blocks).

This method adds one element to the state vector (*x*).

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] df - Size of perturbation to apply. (default: 100e3)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianAddFreqShift(Workspace& ws, const Numeric& df = 100e3) {
  jacobianAddFreqShift(ws, Var::jacobian_quantities(ws),
                       Var::jacobian_agenda(ws), Var::f_grid(ws), df,
                       Var::verbosity(ws));
}

/*! Includes a frequency fit of stretch type in the Jacobian.

Retrieval of deviations between nominal and actual backend
frequencies can be included by this method. The assumption here is
that the deviation varies linearly over the frequency range
(following ARTS basis function for polynomial order 1).

This method adds one element to the state vector (*x*).

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] df - Size of perturbation to apply. (default: 100e3)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianAddFreqStretch(Workspace& ws, const Numeric& df = 100e3) {
  jacobianAddFreqStretch(ws, Var::jacobian_quantities(ws),
                         Var::jacobian_agenda(ws), Var::f_grid(ws), df,
                         Var::verbosity(ws));
}

/*! Includes one magnetic field component in the Jacobian.

The method follows the pattern of other Jacobian methods. The
calculations can only be performed by analytic expressions.

The magnetic field components are retrieved separately, and,
hence, the argument *component* can be  "u", "v", "w",
and "strength".

The number of elements added to the state vector (*x*) is:
   n_g1 * n_g2 * n_g3
where n_g1, n_g2 and n_g3 are the length of GIN *g1*, *g2* and *g3*,
respectively. Here empty vectors should be considered to have a length 1.
The elements are sorted with pressure as innermost loop, followed by
latitude and longitude as outermost loop.

The dB-parameter is only used for Faraday rotation

@author Patrick Eriksson
@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] g1 - Pressure retrieval grid.
@param[in] g2 - Latitude retrieval grid.
@param[in] g3 - Longitude retreival grid.
@param[in] component - Magnetic field component to retrieve (default: "v")
@param[in] dB - Magnetic field perturbation (default: 1.0e-7)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianAddMagField(Workspace& ws, const Vector& g1, const Vector& g2,
                         const Vector& g3, const String& component = "v",
                         const Numeric& dB = 1.0e-7) {
  jacobianAddMagField(ws, Var::jacobian_quantities(ws),
                      Var::jacobian_agenda(ws), Var::atmosphere_dim(ws),
                      Var::p_grid(ws), Var::lat_grid(ws), Var::lon_grid(ws), g1,
                      g2, g3, component, dB, Var::verbosity(ws));
}

/*! Experimental NLTE Jacobian.

Intention: Adds the nlte_field level distribution per atmospheric grid
to the Jacobian.

The number of elements added to the state vector (*x*) is:
   n_g1 * n_g2 * n_g3
where n_g1, n_g2 and n_g3 are the length of GIN *g1*, *g2* and *g3*,
respectively. Here empty vectors should be considered to have a length 1.
The elements are sorted with pressure as innermost loop, followed by
latitude and longitude as outermost loop.

The QuantumIdentifier should identify a single energy level, such as:
"H2O-161 EN J 1 Ka 0 Kc 1", for one of the lower levels in the chains
of transitions of water.  Note that using this method directly is not
best practice, as the quantum identifiers of the levels have to be known
at an early stage in NLTE calculations, and will usually populate the
*nlte_level_identifiers* variable, meaning it is better to use *jacobianAddNLTE*
directly than to individually call this function

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] g1 - Pressure retrieval grid.
@param[in] g2 - Latitude retrieval grid.
@param[in] g3 - Longitude retreival grid.
@param[in] energy_level_identity - Identifier to the eneregy level
@param[in] dx - Perturbation of value if required by method (default: 1.0e-3)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianAddNLTE(Workspace& ws, const Vector& g1, const Vector& g2,
                     const Vector& g3,
                     const QuantumIdentifier& energy_level_identity,
                     const Numeric& dx = 1.0e-3) {
  jacobianAddNLTE(ws, Var::jacobian_quantities(ws), Var::jacobian_agenda(ws),
                  Var::atmosphere_dim(ws), Var::p_grid(ws), Var::lat_grid(ws),
                  Var::lon_grid(ws), g1, g2, g3, energy_level_identity, dx,
                  Var::verbosity(ws));
}

/*! Experimental NLTE Jacobian.  Same as *jacobianAddNLTE* but for
many levels

Adds energy_level_identities.nelem() times as many arguments to *x*
as *jacobianAddNLTE*, ordered as energy_level_identities describes

This method is preferred to *jacobianAddNLTE*, since *energy_level_identities*
is conveniently almost always the same as *nlte_level_identifiers*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] g1 - Pressure retrieval grid.
@param[in] g2 - Latitude retrieval grid.
@param[in] g3 - Longitude retreival grid.
@param[in] energy_level_identities - Identifiers to the eneregy level
@param[in] dx - Perturbation of value if required by method (default: 1.0e-3)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianAddNLTEs(Workspace& ws, const Vector& g1, const Vector& g2,
                      const Vector& g3,
                      const ArrayOfQuantumIdentifier& energy_level_identities,
                      const Numeric& dx = 1.0e-3) {
  jacobianAddNLTEs(ws, Var::jacobian_quantities(ws), Var::jacobian_agenda(ws),
                   Var::atmosphere_dim(ws), Var::p_grid(ws), Var::lat_grid(ws),
                   Var::lon_grid(ws), g1, g2, g3, energy_level_identities, dx,
                   Var::verbosity(ws));
}

/*! Adds sensor pointing zenith angle off-set jacobian.

Retrieval of deviations between nominal and actual zenith angle of
the sensor can be included by this method. The weighing functions
can be calculated in several ways:
   calcmode = "recalc": Recalculation of pencil beam spectra,
      shifted with *dza* from nominal values. A single-sided
      perturbation is applied (towards higher zenith angles).
   calcmode = "interp": Inter/extrapolation of existing pencil
       beam spectra. For this option, allow some extra margins for
       zenith angle grids, to avoid artifacts when extrapolating
       the data (to shifted zenith angles). The average of a
       negative and a positive shift is taken.
The interp option is recommended. It should in general be both
faster and more accurate (due to the double sided disturbance).
In addition, it is less sensitive to the choice of dza (as long
as a small value is applied).

The pointing off-set can be modelled to be time varying. The time
variation is then described by a polynomial (with standard base
functions). For example, a polynomial order of 0 means that the
off-set is constant in time. If the off-set is totally uncorrelated
between the spectra, set the order to -1.

The number of elements added to the state vector (*x*) is
  if poly_order < 0 : length of *sensor_time*
         otherwise : poly_order+1
In the first case, the order in *x* matches *sensor_time*. In the second
case, the coefficient for polynomial order 0 comes first etc.

@author Patrick Eriksson
@author Mattias Ekstrom

@param[in,out] Workspace ws - An ARTS workspace
@param[in] poly_order - Order of polynomial to describe the time variation of
pointing off-sets. (default: 0)
@param[in] calcmode - Calculation method. See above (default: "recalc")
@param[in] dza - Size of perturbation to apply (when applicable). (default:
0.01)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianAddPointingZa(Workspace& ws, const Index& poly_order = 0,
                           const String& calcmode = "recalc",
                           const Numeric& dza = 0.01) {
  jacobianAddPointingZa(ws, Var::jacobian_quantities(ws),
                        Var::jacobian_agenda(ws), Var::sensor_pos(ws),
                        Var::sensor_time(ws), poly_order, calcmode, dza,
                        Var::verbosity(ws));
}

/*! Includes polynomial baseline fit in the Jacobian.

This method deals with retrieval of disturbances of the spectra
that can be described by an additive term, a baseline off-set.

The baseline off-set is here modelled as a polynomial. The
polynomial spans the complete frequency range spanned by
*sensor_response_f_grid* and the method should only of interest for
cases with no frequency gap in the spectra. The default assumption
is that the off-set differs between all spectra, but it can also be
assumed that the off-set is common for all e.g. line-of-sights.

If the simulation/retrieval deals with a single spectrum, the number
of elements added to the state vector (*x*) is poly_order+1. The
coefficient for polynomial order 0 comes first etc. The same is true
if *no_pol_variation*, *no_los_variation* and *no_mblock_variation*
all are set to 1, even if several spectra are involved. Otherwise thenumber of
elements added to *x* depends on the number of spectra and the settings of
*no_pol_variation*, *no_los_variation* and *no_mblock_variation*. The
coefficients of the different polynomial orders are treated as separate
retrieval quantities. That is, the the elements associated with polynomial order
0 are grouped and form together a retrieval quantity. The coefficients for
higher polynomial orders are treated in the same way.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] poly_order - Polynomial order to use for the fit.
@param[in] no_pol_variation - Set to 1 if the baseline off-set is the same for
all Stokes components. (default: 0)
@param[in] no_los_variation - Set to 1 if the baseline off-set is the same for
all line-of-sights (inside each measurement block). (default: 0)
@param[in] no_mblock_variation - Set to 1 if the baseline off-set is the same
for all measurement blocks. (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianAddPolyfit(Workspace& ws, const Index& poly_order,
                        const Index& no_pol_variation = 0,
                        const Index& no_los_variation = 0,
                        const Index& no_mblock_variation = 0) {
  jacobianAddPolyfit(ws, Var::jacobian_quantities(ws), Var::jacobian_agenda(ws),
                     Var::sensor_response_pol_grid(ws),
                     Var::sensor_response_dlos_grid(ws), Var::sensor_pos(ws),
                     poly_order, no_pol_variation, no_los_variation,
                     no_mblock_variation, Var::verbosity(ws));
}

/*! Includes a scattering species in the Jacobian.

For 1D or 2D calculations the latitude and/or longitude grid of
the retrieval field should set to have zero length.

The number of elements added to the state vector (*x*) is:
   n_g1 * n_g2 * n_g3
where n_g1, n_g2 and n_g3 are the length of GIN *g1*, *g2* and *g3*,
respectively. Here empty vectors should be considered to have a length 1.
The elements are sorted with pressure as innermost loop, followed by
latitude and longitude as outermost loop.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] g1 - Pressure retrieval grid.
@param[in] g2 - Latitude retrieval grid.
@param[in] g3 - Longitude retreival grid.
@param[in] species - Name of scattering species, must match one element in
*scat_species*.
@param[in] quantity - Retrieval quantity, e.g. "IWC".

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianAddScatSpecies(Workspace& ws, const Vector& g1, const Vector& g2,
                            const Vector& g3, const String& species,
                            const String& quantity) {
  jacobianAddScatSpecies(ws, Var::jacobian_quantities(ws),
                         Var::jacobian_agenda(ws), Var::atmosphere_dim(ws),
                         Var::p_grid(ws), Var::lat_grid(ws), Var::lon_grid(ws),
                         g1, g2, g3, species, quantity, Var::verbosity(ws));
}

/*! Adds a line shape parameter to the Jacobian calculations. These
are constant over all levels so only a single *x*-value is added

Line function parameter assume the derivatives
of internal pressure broadening and line mixing
functionality follows a simply f(T, T0, X0, X1, X2)
format.  The shape of the function f() is determined by
input catalog; please see the ARTS documentation for more
details

The input are as follows:
    line_identity: Identifier of preferably a single line
    species:       A SpeciesTag, e.g., "O2" or "H2O" for common species.
                   Note that "SELF" and "AIR" tags are used for shape parameters
                   affected by self and air-broadening, respectively.
    variable:      A variable supported by the line, these can be
                      "G0":  Speed-independent pressure broadening
                      "G2":  Speed-dependent pressure broadening
                      "D0":  Speed-independent pressure shift
                      "D2":  Speed-dependent pressure shift
                      "FVC": Frequency of velocity changing collisions
                      "ETA": partial correlation between velocity and
                               rotational state changes due to collisions
                      "Y":   First order line-mixing parameter
                      "G":   Second order line-mixing parameter for strength
                      "DV":  Second order line-mixing parameter for shifting
    coefficient:   A coefficient in the model to compute the above parameters.

Note that we cannot test if the line in question supports the variable and
coefficient at the level of this function, so many errors will only be reported
at a later stage

For other spectroscopic parameters, see *jacobianAddBasicCatalogParameter*.
Also see said function for an example of how to set the QuantumIdentifier

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] line_identity - Line identifier
@param[in] species - Species of interest
@param[in] variable - Variable of interest
@param[in] coefficient - Coefficient of interest

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianAddShapeCatalogParameter(Workspace& ws,
                                      const QuantumIdentifier& line_identity,
                                      const String& species,
                                      const String& variable,
                                      const String& coefficient) {
  jacobianAddShapeCatalogParameter(
      ws, Var::jacobian_quantities(ws), Var::jacobian_agenda(ws), line_identity,
      species, variable, coefficient, Var::verbosity(ws));
}

/*! See *jacobianAddShapeCatalogParameter* for information on
the GIN parameters

This function accepts the same input but for lists of data.
The function loops over each input list
individually and appends the information to *jacobian_quantities*.

Special "ALL" for 1 length *variables* and *coefficients* are
allowed to compute all variables/coefficients in the order described
in the description of *jacobianAddShapeCatalogParameter*

For example, if *line_identities* have length 5, *species* length 4,
*variables* length 3, and *coefficients* length 2, there will be
5*4x3x2 = 120 new additions to *jacobian_quantities* in the order:
        [{line_identities[0], species[0], variables[0] coefficients[0]}]
        [{line_identities[0], species[0], variables[0] coefficients[1]}]
        [{line_identities[0], species[0], variables[1] coefficients[0]}]
        [{line_identities[0], species[0], variables[1] coefficients[1]}]
        [{line_identities[0], species[0], variables[2] coefficients[0]}]
        [{line_identities[0], species[0], variables[2] coefficients[1]}]
        [{line_identities[0], species[1], variables[0] coefficients[0]}]
        ...
        [{line_identities[4], species[3], variables[1] coefficients[1]}]
        [{line_identities[4], species[3], variables[2] coefficients[0]}]
        [{line_identities[4], species[3], variables[2] coefficients[1]}]
or in words: lines first, then species, then variables, then coefficients

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] line_identities - List of line identifiers
@param[in] species - List of species of interest
@param[in] variables - List of variables of interest
@param[in] coefficients - List of coefficients of interest

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianAddShapeCatalogParameters(
    Workspace& ws, const ArrayOfQuantumIdentifier& line_identities,
    const ArrayOfString& species, const ArrayOfString& variables,
    const ArrayOfString& coefficients) {
  jacobianAddShapeCatalogParameters(
      ws, Var::jacobian_quantities(ws), Var::jacobian_agenda(ws),
      line_identities, species, variables, coefficients, Var::verbosity(ws));
}

/*! Includes sinusoidal baseline fit in the Jacobian.

Works as *jacobianAddPolyfit*, beside that a series of sine and
cosine terms are used for the baseline fit.

For each value in *period_lengths one sine and one cosine term are
included (in mentioned order). By these two terms the amplitude and
"phase" for each period length can be determined. The sine and
cosine terms have value 0 and 1, respectively, for first frequency.

If the simulation/retrieval deals with a single spectrum, the number
of elements added to the state vector (*x*) is 2*nperiods, where
nperiods is the length of *period_lengths*. The same is true
if *no_pol_variation*, *no_los_variation* and *no_mblock_variation*
all are set to 1, even if several spectra are involved. Otherwise thenumber of
elements added to *x* depends on the number of spectra and the settings of
*no_pol_variation*, *no_los_variation* and *no_mblock_variation*. The sine and
cosine terms for each period length are treated as a  separate retrieval
quantities. That is, the the elements associated with the first period length
are grouped and form together a retrieval quantity, etc. Inside each retrieval
quantity the pairs of sine and cosine terms are kept together, in given order.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] period_lengths - Period lengths of the fit.
@param[in] no_pol_variation - Set to 1 if the baseline off-set is the same for
all Stokes components. (default: 0)
@param[in] no_los_variation - Set to 1 if the baseline off-set is the same for
all line-of-sights (inside each measurement block). (default: 0)
@param[in] no_mblock_variation - Set to 1 if the baseline off-set is the same
for all measurement blocks. (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianAddSinefit(Workspace& ws, const Vector& period_lengths,
                        const Index& no_pol_variation = 0,
                        const Index& no_los_variation = 0,
                        const Index& no_mblock_variation = 0) {
  jacobianAddSinefit(ws, Var::jacobian_quantities(ws), Var::jacobian_agenda(ws),
                     Var::sensor_response_pol_grid(ws),
                     Var::sensor_response_dlos_grid(ws), Var::sensor_pos(ws),
                     period_lengths, no_pol_variation, no_los_variation,
                     no_mblock_variation, Var::verbosity(ws));
}

/*! Includes a special absorption species in the Jacobian.

Similar to *jacobianAddAbsSpecies* but only for number densities.

Species allowed are:
    "electrons"
    "particulates"

Note that the average of all particulates are used to scale its
*jacobian*, so this method works best when only one type of
particulate is being used, i.e., when *scat_data* has only one
scattering species.

The number of elements added to the state vector (*x*) is:
   n_g1 * n_g2 * n_g3
where n_g1, n_g2 and n_g3 are the length of GIN *g1*, *g2* and *g3*,
respectively. Here empty vectors should be considered to have a length 1.
The elements are sorted with pressure as innermost loop, followed by
latitude and longitude as outermost loop.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] g1 - Pressure retrieval grid.
@param[in] g2 - Latitude retrieval grid.
@param[in] g3 - Longitude retreival grid.
@param[in] species - The species of the retrieval quantity.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianAddSpecialSpecies(Workspace& ws, const Vector& g1,
                               const Vector& g2, const Vector& g3,
                               const String& species) {
  jacobianAddSpecialSpecies(
      ws, Var::jacobian_quantities(ws), Var::jacobian_agenda(ws),
      Var::atmosphere_dim(ws), Var::p_grid(ws), Var::lat_grid(ws),
      Var::lon_grid(ws), g1, g2, g3, species, Var::verbosity(ws));
}

/*! Includes a surface quantity in the Jacobian.

The quantity is specified by the GIN-variable *quantity*. The name
of the quantity must match the name used in *surface_props_names*.

For 1D or 2D calculations the latitude and/or longitude grid of
the retrieval field should set to have zero length.

The number of elements added to the state vector (*x*) is:
   n_g1 * n_g2
where n_g1 and n_g2 are the length of GIN *g1* and *g2*, respectively.
Here empty vectors should be considered to have a length 1.
The elements are sorted with latitude as innermost loop and longitude
as outermost loop.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] g1 - Latitude retrieval grid.
@param[in] g2 - Longitude retreival grid.
@param[in] quantity - Retrieval quantity, e.g. "Wind speed".

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianAddSurfaceQuantity(Workspace& ws, const Vector& g1,
                                const Vector& g2, const String& quantity) {
  jacobianAddSurfaceQuantity(ws, Var::jacobian_quantities(ws),
                             Var::jacobian_agenda(ws), Var::atmosphere_dim(ws),
                             Var::lat_grid(ws), Var::lon_grid(ws), g1, g2,
                             quantity, Var::verbosity(ws));
}

/*! Includes atmospheric temperatures in the Jacobian.

The calculations are performed by (semi-)analytical expressions.
Hydrostatic equilibrium (HSE) can be included.

The analytical calculation approach neglects so far refraction
totally, but considers the local effect of HSE.
The later should be accaptable for observations around zenith and
nadir. There is no warning if the method is applied incorrectly,
with respect to these issues. Note that the argument *hse* of this
WSM only refers to the Jacobian calculation, if the model and/or
retrieved atmosphere actually fulfils HSE or not is governed in
other manners.

The calculations (both options) assume that gas species are defined
in VMR (a change in temperature then changes the number density).
This has the consequence that retrieval of temperatures and number
density can not be mixed. Neither any warning here!

The number of elements added to the state vector (*x*) is:
   n_g1 * n_g2 * n_g3
where n_g1, n_g2 and n_g3 are the length of GIN *g1*, *g2* and *g3*,
respectively. Here empty vectors should be considered to have a length 1.
The elements are sorted with pressure as innermost loop, followed by
latitude and longitude as outermost loop.

@author Mattias Ekstrom
@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] g1 - Pressure retrieval grid.
@param[in] g2 - Latitude retrieval grid.
@param[in] g3 - Longitude retreival grid.
@param[in] hse - Flag to assume HSE or not ("on" or "off"). (default: "on")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianAddTemperature(Workspace& ws, const Vector& g1, const Vector& g2,
                            const Vector& g3, const String& hse = "on") {
  jacobianAddTemperature(ws, Var::jacobian_quantities(ws),
                         Var::jacobian_agenda(ws), Var::atmosphere_dim(ws),
                         Var::p_grid(ws), Var::lat_grid(ws), Var::lon_grid(ws),
                         g1, g2, g3, hse, Var::verbosity(ws));
}

/*! Includes one atmospheric wind component in the Jacobian.

The method follows the pattern of other Jacobian methods. The
calculations can only be performed by analytic expressions.
Some lower level function depends on frequency perturbations,
however, so therefore a frequency perturbation df is required
and as a consequence *abs_f_interp_order* must be > 0.

The wind field components are retrieved separately, and,
hence, the argument *component* can be "u", "v" or "w"
for vector components, or just "strength" for total wind speed.

The number of elements added to the state vector (*x*) is:
   n_g1 * n_g2 * n_g3
where n_g1, n_g2 and n_g3 are the length of GIN *g1*, *g2* and *g3*,
respectively. Here empty vectors should be considered to have a length 1.
The elements are sorted with pressure as innermost loop, followed by
latitude and longitude as outermost loop.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] g1 - Pressure retrieval grid.
@param[in] g2 - Latitude retrieval grid.
@param[in] g3 - Longitude retrieval grid.
@param[in] component - Wind component to retrieve (default: "v")
@param[in] dfrequency - This is the frequency perturbation (default: 0.1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianAddWind(Workspace& ws, const Vector& g1, const Vector& g2,
                     const Vector& g3, const String& component = "v",
                     const Numeric& dfrequency = 0.1) {
  jacobianAddWind(ws, Var::jacobian_quantities(ws), Var::jacobian_agenda(ws),
                  Var::atmosphere_dim(ws), Var::p_grid(ws), Var::lat_grid(ws),
                  Var::lon_grid(ws), g1, g2, g3, component, dfrequency,
                  Var::verbosity(ws));
}

/*! Applies adjustments and transformations on *jacobian*.

The method handles two tasks:
1. The retrieval transformations set by the user can not be applied
onthe  Jacobian inside *yCalc*. Transformations are instead applied
by calling this method.
2. It applies required adjustments of the Jacoboan. So far there is
only one possible adjustment. If any absorption species uses the "rel"
unit, an adjustment is needed for later iterations of the inversion.

If no tranformations are selected and the "rel" option is not used at
all, there is no need to call this method(, but you can still include it
without causing any error, the calculations will just be a bit slower).
Otherwise, this method should be called, typically as part of
*inversion_iterate_agenda*.

The method accepts if *jacobian* is empty, and then does, nothing.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianAdjustAndTransform(Workspace& ws) {
  jacobianAdjustAndTransform(Var::jacobian(ws), Var::jacobian_quantities(ws),
                             Var::x(ws), Var::verbosity(ws));
}

/*! This function doesn't do anything. It just exists to satisfy
the input and output requirement of the *jacobian_agenda*.

This method is added to *jacobian_agenda* by *jacobianAddAbsSpecies*
and some similar methods, and it should normally not be called by
the user.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianCalcDoNothing(Workspace& ws) {
  jacobianCalcDoNothing(Var::jacobian(ws), Var::mblock_index(ws), Var::iyb(ws),
                        Var::yb(ws), Var::verbosity(ws));
}

/*! Calculates frequency shift jacobians by interpolation
of *iyb*.

This function is added to *jacobian_agenda* by jacobianAddFreqShift
and should normally not be called by the user.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianCalcFreqShift(Workspace& ws) {
  jacobianCalcFreqShift(Var::jacobian(ws), Var::mblock_index(ws), Var::iyb(ws),
                        Var::yb(ws), Var::stokes_dim(ws), Var::f_grid(ws),
                        Var::mblock_dlos_grid(ws), Var::sensor_response(ws),
                        Var::jacobian_quantities(ws), Var::verbosity(ws));
}

/*! Calculates frequency stretch jacobians by interpolation
of *iyb*.

This function is added to *jacobian_agenda* by jacobianAddFreqStretch
and should normally not be called by the user.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianCalcFreqStretch(Workspace& ws) {
  jacobianCalcFreqStretch(
      Var::jacobian(ws), Var::mblock_index(ws), Var::iyb(ws), Var::yb(ws),
      Var::stokes_dim(ws), Var::f_grid(ws), Var::mblock_dlos_grid(ws),
      Var::sensor_response(ws), Var::sensor_response_pol_grid(ws),
      Var::sensor_response_f_grid(ws), Var::sensor_response_dlos_grid(ws),
      Var::jacobian_quantities(ws), Var::verbosity(ws));
}

/*! Calculates zenith angle pointing deviation jacobians by
inter-extrapolation of *iyb*.

This function is added to *jacobian_agenda* by
jacobianAddPointingZa and should normally not be
called by the user.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianCalcPointingZaInterp(Workspace& ws) {
  jacobianCalcPointingZaInterp(
      Var::jacobian(ws), Var::mblock_index(ws), Var::iyb(ws), Var::yb(ws),
      Var::stokes_dim(ws), Var::f_grid(ws), Var::sensor_los(ws),
      Var::mblock_dlos_grid(ws), Var::sensor_response(ws), Var::sensor_time(ws),
      Var::jacobian_quantities(ws), Var::verbosity(ws));
}

/*! Calculates zenith angle pointing deviation jacobians by
recalulation of *iyb*.

This function is added to *jacobian_agenda* by
jacobianAddPointingZa and should normally not be
called by the user.

@author Mattias Ekstrom
@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianCalcPointingZaRecalc(Workspace& ws) {
  jacobianCalcPointingZaRecalc(
      ws, Var::jacobian(ws), Var::mblock_index(ws), Var::iyb(ws), Var::yb(ws),
      Var::atmosphere_dim(ws), Var::nlte_field(ws), Var::cloudbox_on(ws),
      Var::stokes_dim(ws), Var::f_grid(ws), Var::sensor_pos(ws),
      Var::sensor_los(ws), Var::transmitter_pos(ws), Var::mblock_dlos_grid(ws),
      Var::sensor_response(ws), Var::sensor_time(ws), Var::iy_unit(ws),
      Var::iy_main_agenda(ws), Var::geo_pos_agenda(ws),
      Var::jacobian_quantities(ws), Var::verbosity(ws));
}

/*! Calculates jacobians for polynomial baseline fit.

This function is added to *jacobian_agenda* by jacobianAddPolyfit
and should normally not be called by the user.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] poly_coeff - Polynomial coefficient to handle.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianCalcPolyfit(Workspace& ws, const Index& poly_coeff) {
  jacobianCalcPolyfit(
      Var::jacobian(ws), Var::mblock_index(ws), Var::iyb(ws), Var::yb(ws),
      Var::sensor_response(ws), Var::sensor_response_pol_grid(ws),
      Var::sensor_response_f_grid(ws), Var::sensor_response_dlos_grid(ws),
      Var::jacobian_quantities(ws), poly_coeff, Var::verbosity(ws));
}

/*! Calculates jacobians for sinusoidal baseline fit.

This function is added to *jacobian_agenda* by jacobianAddPolyfit
and should normally not be called by the user.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] period_index - Index among the period length specified for
add-method.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianCalcSinefit(Workspace& ws, const Index& period_index) {
  jacobianCalcSinefit(
      Var::jacobian(ws), Var::mblock_index(ws), Var::iyb(ws), Var::yb(ws),
      Var::sensor_response(ws), Var::sensor_response_pol_grid(ws),
      Var::sensor_response_f_grid(ws), Var::sensor_response_dlos_grid(ws),
      Var::jacobian_quantities(ws), period_index, Var::verbosity(ws));
}

/*! Closes the array of retrieval quantities and prepares for
calculation of the Jacobian matrix.

This function closes the *jacobian_quantities* array and sets
*jacobian_do* to 1.

Retrieval quantities should not be added after a call to this WSM.
No calculations are performed here.

@author Mattias Ekstrom

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianClose(Workspace& ws) {
  jacobianClose(ws, Var::jacobian_do(ws), Var::jacobian_agenda(ws),
                Var::jacobian_quantities(ws), Var::verbosity(ws));
}

/*! Sets *jacobian* based on the difference vetween two measurement vectors.

This function assumes that *y_pert* contains a measurement calculated
with some variable perturbed, in comparison to the calculation
behind *y*. The function takes the differences between *y_pert*
and *y* to form a numerical derived estimate of *jacobian*.
This gives a Jacobian wit a single column.

*jacobian* equals here: (y_pert-y)/pert_size.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] y_pert - Perturbed measurement vector
@param[in] pert_size - Size of perturbation behind spectra in *ybatch*.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianFromTwoY(Workspace& ws, const Vector& y_pert,
                      const Numeric& pert_size) {
  jacobianFromTwoY(Var::jacobian(ws), Var::y(ws), y_pert, pert_size,
                   Var::verbosity(ws));
}

/*! Sets *jacobian* based on perturbation calcuations.

This function assumes that *ybatch* contains spectra calculated
with some variable perturbed, in comparison to the calculation
behind *y*. The function takes the differences between *ybatch*
and *y* to form a numerical derived estimate of *jacobian*.

Column i of *jacobian* equals: (ybatch[i]-y)/pert_size.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] pert_size - Size of perturbation behind spectra in *ybatch*.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianFromYbatch(Workspace& ws, const Numeric& pert_size) {
  jacobianFromYbatch(Var::jacobian(ws), Var::ybatch(ws), Var::y(ws), pert_size,
                     Var::verbosity(ws));
}

/*! Initialises the variables connected to the Jacobian matrix.

This function initialises the *jacobian_quantities* array so
that retrieval quantities can be added to it. Accordingly, it has
to be called before any calls to jacobianAddTemperature or
similar methods.

The Jacobian quantities are initialised to be empty.

@author Mattias Ekstrom

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianInit(Workspace& ws) {
  jacobianInit(Var::jacobian_quantities(ws), Var::jacobian_agenda(ws),
               Var::verbosity(ws));
}

/*! Makes mandatory initialisation of some jacobian variables.

Some clear-sky jacobian WSVs must be initialised even if no such
calculations will be performed.  This is handled with this method.
That is, this method must be called when no clear-sky jacobians
will be calculated (even if cloudy-sky jacobians are calculated!).

Sets *jacobian_do* to 0.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianOff(Workspace& ws) {
  jacobianOff(Var::jacobian_do(ws), Var::jacobian_agenda(ws),
              Var::jacobian_quantities(ws), Var::verbosity(ws));
}

/*! Adds an affine transformation of the last element of
*jacobian_quantities*.

See *jacobianSetFuncTransformation* for  a general description of how
retrieval transformations are defined. Transformations are not applied by
methods such as*yCalc*. Instead, the method *jacobianAdjustAndTransform*
must be called to activate the transformations.

The affine transformation is specified by a transformation matrix, A,
and an offset vector, b. These two are applied as described in
*jacobianSetFuncTransformation*.

The transformations is applied as
   x = A * ( z - b )
where z is the retrieval quantity on the standard retrieval grids
and x is the final state vector.

So far, the following must be true for valid A-matrices
   z = A'*x + b
That is, the reversed transformation is given by A transposed.

This method must only be called if an affine transformation is wanted.
Default is to make no such tranformation at all.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[in] transformation_matrix - The transformation matrix A
@param[in] offset_vector - The offset vector b

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianSetAffineTransformation(Workspace& ws,
                                     const Matrix& transformation_matrix,
                                     const Vector& offset_vector) {
  jacobianSetAffineTransformation(Var::jacobian_quantities(ws),
                                  transformation_matrix, offset_vector,
                                  Var::verbosity(ws));
}

/*! Sets the functional transformation of the last element of
*jacobian_quantities*.

See below for a general description of how retrieval transformations
are defined. Transformations are not applied by methods such as*yCalc*.
Instead, the method *jacobianAdjustAndTransform* must be called to
activate the transformations.

The following transformations can be selected (by *transformation_func*):
   log   : The natural logarithm
   log10 : The base-10 logarithm
   atanh : Area hyperbolic tangent
   none  : No transformation at all

This method needs only to be called if a functional transformation
is wanted. Default is to make no such tranformation at all (i.e.
the option "none" exists only for reasons of flexibility).

The log-options are applied as log(z-z_min) and log10(z-z_min).
The default for *z_min* is zero, but by changing it the lower limit
for z can be changed. Note that *z_min* becomes the lower limit for
allowed values of z. The GIN *z_max* is here ignored.

For the atanh-option, also *z_max* is considered. This transformation
is applied as atanh((2(z-z_min)/(z_max-z_min))-1). As above,*z_min*
is lower limit for allowed values of z. On the other hand, *z_max*
eines the upper limit for z.

The GIN *transformation_func* is so far only used for atanh. The parameter
specifies the maximum allowed value allowed for u. That is, the valid
range for u becomes ]0,tfunc_parameter[. Note that log and log10
demands/ensures that u > 0, but implies no upper limit.

General handling of retrieval units and transformations:
---
Default is that quantities are retrieved as defined in ARTS, but
both some unit conversion and transformations are provided. These
operations are applied as:
   x = A * ( f(u(z)) - b )
where
   z is the quantity as defined ARTS
   u represents the change of unit
   f is the transformation function
   A and b define together an affine transformation
   x is the retrieved quantity
For example, this systen allows to retrive a principal component
representation (A and b) of the log (f) of relative humidity (u).

Change of unit is selected by the quantity specific jacobian-add
methods (so far only at hand for gas species).

Activating a transformation function is done by this method. Note
that the functions are defined as the transformation from z to x.
For more details on affine transformations, see
*jacobianSetAffineTransformation*.

@author Patrick Eriksson
@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[in] transformation_func - The transformation function.
@param[in] z_min - Lower limit of z. (default: 0)
@param[in] z_max - Upper limit of z. (default: -99e99)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void jacobianSetFuncTransformation(Workspace& ws,
                                   const String& transformation_func,
                                   const Numeric& z_min = 0,
                                   const Numeric& z_max = -99e99) {
  jacobianSetFuncTransformation(Var::jacobian_quantities(ws),
                                transformation_func, z_min, z_max,
                                Var::verbosity(ws));
}

/*! Sets *lat_grid* according to given raw atmospheric field's lat_grid.
Similar to *p_gridFromZRaw*, but acting on a generic *GriddedField3*
(e.g., a wind or magnetic field component).

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] field_raw - A raw atmospheric field.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void lat_gridFromRawField(Workspace& ws, const GriddedField3& field_raw) {
  lat_gridFromRawField(Var::lat_grid(ws), field_raw, Var::verbosity(ws));
}

/*! Sets *lat_grid* according to input atmosphere's *z_field_raw*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void lat_gridFromZRaw(Workspace& ws) {
  lat_gridFromZRaw(Var::lat_grid(ws), Var::z_field_raw(ws), Var::verbosity(ws));
}

/*! Checks that the line-by-line parameters are OK.

On failure, will throw.  On success, lbl_checked evals as true

Note that checks may become more stringent as ARTS evolves, especially for
"new" options.  This test might succeed in one version of ARTS but fail
in later versions

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void lbl_checkedCalc(Workspace& ws) {
  lbl_checkedCalc(Var::lbl_checked(ws), Var::abs_lines_per_species(ws),
                  Var::abs_species(ws), Var::isotopologue_ratios(ws),
                  Var::partition_functions(ws), Var::verbosity(ws));
}

/*! Computes the line irradiance and line transmission

Presently only works for 1D atmospheres

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] df - relative frequency to line center
@param[in] nz - number of zeniths
@param[in] nf - number of frequencies per line
@param[in] r - Distance assumed when computing local (1-T) (default: 1.0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void line_irradianceCalcForSingleSpeciesNonOverlappingLinesPseudo2D(
    Workspace& ws, const Numeric& df, const Index& nz, const Index& nf,
    const Numeric& r = 1.0) {
  line_irradianceCalcForSingleSpeciesNonOverlappingLinesPseudo2D(
      ws, Var::line_irradiance(ws), Var::line_transmission(ws),
      Var::abs_species(ws), Var::abs_lines_per_species(ws), Var::nlte_field(ws),
      Var::vmr_field(ws), Var::t_field(ws), Var::z_field(ws), Var::p_grid(ws),
      Var::refellipsoid(ws), Var::surface_props_data(ws),
      Var::iy_main_agenda(ws), Var::ppath_agenda(ws), Var::iy_space_agenda(ws),
      Var::iy_surface_agenda(ws), Var::iy_cloudbox_agenda(ws),
      Var::propmat_clearsky_agenda(ws), df, nz, nf, r, Var::verbosity(ws));
}

/*! Sets *lon_grid* according to given raw atmospheric field's lat_grid.
Similar to *p_gridFromZRaw*, but acting on a generic *GriddedField3*
(e.g., a wind or magnetic field component).

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] field_raw - A raw atmospheric field.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void lon_gridFromRawField(Workspace& ws, const GriddedField3& field_raw) {
  lon_gridFromRawField(Var::lon_grid(ws), field_raw, Var::verbosity(ws));
}

/*! Sets *lon_grid* according to input atmosphere's *z_field_raw*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void lon_gridFromZRaw(Workspace& ws) {
  lon_gridFromZRaw(Var::lon_grid(ws), Var::z_field_raw(ws), Var::verbosity(ws));
}

/*! Gives *mblock_dlos_grid* roughly circular coverage, with uniform spacing.

The method considers points on a regular grid with a spacing set by
GIN *spacing*. All points inside a radius from (0,0) are included in
*mblock_dlos_grid*. The positions in *mblock_dlos_grid* thus covers
a roughly circular domain, and cover the same solid beam angle.
The radius is adjusted according to *spacing' and *centre*, but is
ensured to be >= *width*.

Note that the method assumes that width is small and the solid beam
angle does not change with distance from (0.0).

Defualt is to consider grid positions of ..., -spacing/2, spacing/2, ...
If you want to have (0,0) as a point in *mblock_dlos_grid*, change
*centre* from its default value.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] spacing - The angular spacing between points.
@param[in] width - The minimum distance from (0,0) to cover.
@param[in] centre - Set to 1 to place a point at (0,0). (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void mblock_dlos_gridUniformCircular(Workspace& ws, const Numeric& spacing,
                                     const Numeric& width,
                                     const Index& centre = 0) {
  mblock_dlos_gridUniformCircular(Var::mblock_dlos_grid(ws), spacing, width,
                                  centre, Var::verbosity(ws));
}

/*! Gives *mblock_dlos_grid* rectangular coverage, with uniform spacing.

The method creates an equidistant rectangular grid. The width in zenith
and azimuth can differ. Note that selected widths are half-widths (i.e.
distance from (0,0), and refers to the mimumum value allowed. The actual
width depends on values selected for *spacing* and *centre*.

Defualt is to consider grid positions of ..., -spacing/2, spacing/2, ...
If you want to have (0,0) as a point in *mblock_dlos_grid*, change
*centre* from its default value.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] spacing - The angular spacing between points.
@param[in] za_width - Min value of half-width in zenith angle direction.
@param[in] aa_width - Min value of half-width in azimuth angle direction.
@param[in] centre - Set to 1 to place a point at (0,0). (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void mblock_dlos_gridUniformRectangular(Workspace& ws, const Numeric& spacing,
                                        const Numeric& za_width,
                                        const Numeric& aa_width,
                                        const Index& centre = 0) {
  mblock_dlos_gridUniformRectangular(Var::mblock_dlos_grid(ws), spacing,
                                     za_width, aa_width, centre,
                                     Var::verbosity(ws));
}

/*! Makes mc_antenna (used by MCGeneral) a 2D Gaussian pattern.

The gaussian antenna pattern is determined by *za_sigma* and
*aa_sigma*, which represent the standard deviations in the
uncorrelated bivariate normal distribution.

@author Cory Davis

@param[in,out] Workspace ws - An ARTS workspace
@param[in] za_sigma - Width in the zenith angle dimension as described above.
@param[in] aa_sigma - Width in the azimuth angle dimension as described above.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void mc_antennaSetGaussian(Workspace& ws, const Numeric& za_sigma,
                           const Numeric& aa_sigma) {
  mc_antennaSetGaussian(Var::mc_antenna(ws), za_sigma, aa_sigma,
                        Var::verbosity(ws));
}

/*! Makes mc_antenna (used by MCGeneral) a 2D Gaussian pattern.

The gaussian antenna pattern is determined by *za_fwhm* and
*aa_fwhm*, which represent the full width half maximum (FWHM)
of the antenna response, in the zenith and azimuthal planes.

@author Cory Davis

@param[in,out] Workspace ws - An ARTS workspace
@param[in] za_fwhm - Width in the zenith angle dimension as described above.
@param[in] aa_fwhm - Width in the azimuth angle dimension as described above.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void mc_antennaSetGaussianByFWHM(Workspace& ws, const Numeric& za_fwhm,
                                 const Numeric& aa_fwhm) {
  mc_antennaSetGaussianByFWHM(Var::mc_antenna(ws), za_fwhm, aa_fwhm,
                              Var::verbosity(ws));
}

/*! Makes mc_antenna (used by MCGeneral) a pencil beam.

This WSM makes the subsequent MCGeneral WSM perform pencil beam
RT calculations.

@author Cory Davis

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void mc_antennaSetPencilBeam(Workspace& ws) {
  mc_antennaSetPencilBeam(Var::mc_antenna(ws), Var::verbosity(ws));
}

/*! Retrieve nbooks from given variable and store the value in the
workspace variable *nbooks*

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] v - Variable to get the number of books from.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void nbooksGet(Workspace& ws, const Any_0& v) {
  nbooksGet(Var::nbooks(ws), v, Var::verbosity(ws));
}

/*! Retrieve ncols from given variable and store the value in the
workspace variable *ncols*

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] v - Variable to get the number of columns from.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void ncolsGet(Workspace& ws, const Any_0& v) {
  ncolsGet(Var::ncols(ws), v, Var::verbosity(ws));
}

/*! Retrieve nelem from given variable and store the value in the
variable *nelem*.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] v - Variable to get the number of elements from.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void nelemGet(Workspace& ws, const Any_0& v) {
  nelemGet(Var::nelem(ws), v, Var::verbosity(ws));
}

/*! Retrieve nlibraries from given variable and store the value in the
workspace variable *nlibraries*

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] v - Variable to get the number of libraries from.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void nlibrariesGet(Workspace& ws, const Tensor7& v) {
  nlibrariesGet(Var::nlibraries(ws), v, Var::verbosity(ws));
}

/*! Disable Non-LTE calculations.

The variables are set as follows:
   nlte_field             : Empty.
   nlte_level_identifiers : Empty.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void nlteOff(Workspace& ws) {
  nlteOff(Var::nlte_do(ws), Var::nlte_field(ws),
          Var::nlte_level_identifiers(ws), Var::verbosity(ws));
}

/*! Turns on NTLE calculations.

Takes the quantum identifers for NLTE temperatures and matches it to
lines in *abs_lines_per_species*.  *abs_species* must be set and is used
to speed up calculations.  After the function is done,  all affected
lines in *abs_lines_per_species* will have an internal tag to the relevant
quantum identifier, which is a requirement for deeper code.

If vibrational_energies is input it must match *nlte_level_identifiers*
in length.  The vibrational energies of the affected lines will then be
set by the function.  Otherwise, it is assumed the vibrational energies
are set by another method.  If they are not set, calculations will complain
later on while running deeper code.

For now only vibrational energy states are assumed to be able to be in
non-LTE conditions.  The *QuantumIdentifier* for an energy state in ARTS
can look like:
        "CO2-626 EN v1 0/1 v2 1/1 l2 1/1 v3 0/1 r 1/1"
and the matching will match ALL lines with the above.  Note then that if, e.g.,
the "v1 0/1" term was removed from the above, then ARTS will assume that
"v1" is not part of the level of energy state of interest, so lines
of different "v1" will be matched as the same state.  If a line is matched
to more than one energy state, errors should be thrown, but be careful.

Set type of population to change computations and expected input as:
        LTE: Compute population by ratios found from LTE temperatures
        TV:  Compute population by ratios found from NLTE vibrational
temperatures ND:  Compute population by ratios found from NLTE number densities

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void nlteSetByQuantumIdentifiers(Workspace& ws) {
  nlteSetByQuantumIdentifiers(Var::nlte_do(ws), Var::abs_lines_per_species(ws),
                              Var::nlte_field(ws), Var::verbosity(ws));
}

/*! NLTE field for a simple setup.

This will solve for *nlte_field* in the input atmosphere.
The solver depends on the lines not overlapping and that there
is only a single species in the atmosphere.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] df - relative frequency to line center
@param[in] convergence_limit - max relative change in ratio of level to stop
iterations (default: 1e-6)
@param[in] nz - number of zenith angles
@param[in] nf - number of frequency grid-points per line
@param[in] dampened - use transmission dampening or not
@param[in] iteration_limit - max number of iterations before defaul break of
iterations (default: 20)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void nlte_fieldForSingleSpeciesNonOverlappingLines(
    Workspace& ws, const Numeric& df, const Index& nz, const Index& nf,
    const Index& dampened, const Numeric& convergence_limit = 1e-6,
    const Index& iteration_limit = 20) {
  nlte_fieldForSingleSpeciesNonOverlappingLines(
      ws, Var::nlte_field(ws), Var::abs_species(ws),
      Var::abs_lines_per_species(ws), Var::collision_coefficients(ws),
      Var::collision_line_identifiers(ws), Var::isotopologue_ratios(ws),
      Var::iy_main_agenda(ws), Var::ppath_agenda(ws), Var::iy_space_agenda(ws),
      Var::iy_surface_agenda(ws), Var::iy_cloudbox_agenda(ws),
      Var::propmat_clearsky_agenda(ws), Var::water_p_eq_agenda(ws),
      Var::vmr_field(ws), Var::t_field(ws), Var::z_field(ws), Var::p_grid(ws),
      Var::atmosphere_dim(ws), Var::refellipsoid(ws),
      Var::surface_props_data(ws), Var::nlte_do(ws), df, convergence_limit, nz,
      nf, dampened, iteration_limit, Var::verbosity(ws));
}

/*! Sets NLTE values manually

Touch

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] data - Vibrational data [nlevels, np, nlat, nlon]

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void nlte_fieldFromRaw(Workspace& ws, const Tensor4& data) {
  nlte_fieldFromRaw(Var::nlte_field(ws), Var::nlte_level_identifiers(ws),
                    Var::nlte_vibrational_energies(ws), data,
                    Var::verbosity(ws));
}

/*! Rescale NLTE field to expected total distribution amongst levels

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] s - Scaling (e.g., 0.75 for only orth-water on Earth)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void nlte_fieldRescalePopulationLevels(Workspace& ws, const Numeric& s) {
  nlte_fieldRescalePopulationLevels(Var::nlte_field(ws), s, Var::verbosity(ws));
}

/*! Turns on NTLE calculations.

Sets NLTE ratios to those expected for LTE calculations
with a known partition function

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void nlte_fieldSetLteExternalPartitionFunction(Workspace& ws) {
  nlte_fieldSetLteExternalPartitionFunction(
      Var::nlte_do(ws), Var::nlte_field(ws), Var::abs_lines_per_species(ws),
      Var::nlte_level_identifiers(ws), Var::partition_functions(ws),
      Var::t_field(ws), Var::verbosity(ws));
}

/*! Turns on NTLE calculations.

Sets NLTE ratios to those expected for LTE calculations
with estimation of the partition function as the sum of all
states of a species

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void nlte_fieldSetLteInternalPartitionFunction(Workspace& ws) {
  nlte_fieldSetLteInternalPartitionFunction(
      Var::nlte_do(ws), Var::nlte_field(ws), Var::abs_lines_per_species(ws),
      Var::nlte_level_identifiers(ws), Var::t_field(ws), Var::verbosity(ws));
}

/*! Turn NLTE absorption per species into the source function by multiplying
NLTE absorption per species with the LTE Planck source function.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void nlte_sourceFromTemperatureAndSrcCoefPerSpecies(Workspace& ws) {
  nlte_sourceFromTemperatureAndSrcCoefPerSpecies(
      Var::nlte_source(ws), Var::dnlte_dx_source(ws), Var::nlte_dsource_dx(ws),
      Var::src_coef_per_species(ws), Var::dsrc_coef_dx(ws),
      Var::jacobian_quantities(ws), Var::f_grid(ws), Var::rtp_temperature(ws),
      Var::verbosity(ws));
}

/*! Retrieve npages from given variable and store the value in the
workspace variable *npages*

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] v - Variable to get the number of pages from.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void npagesGet(Workspace& ws, const Any_0& v) {
  npagesGet(Var::npages(ws), v, Var::verbosity(ws));
}

/*! Retrieve nrows from given variable and store the value in the
workspace variable *nrows*

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] v - Variable to get the number of rows from.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void nrowsGet(Workspace& ws, const Any_0& v) {
  nrowsGet(Var::nrows(ws), v, Var::verbosity(ws));
}

/*! Retrieve nshelves from given variable and store the value in the
workspace variable *nshelves*

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] v - Variable to get the number of shelves from.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void nshelvesGet(Workspace& ws, const Any_0& v) {
  nshelvesGet(Var::nshelves(ws), v, Var::verbosity(ws));
}

/*! Retrieve nvitrines from given variable and store the value in the
workspace variable *nvitrines*

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] v - Variable to get the number of vitrines from.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0>
void nvitrinesGet(Workspace& ws, const Any_0& v) {
  nvitrinesGet(Var::nvitrines(ws), v, Var::verbosity(ws));
}

/*! Calculates bulk absorption extinction at one atmospheric grid point.

This WSM sums up the monochromatic absorption vectors and
extinction matrices of all scattering elements (*abs_vec_spt* and
*ext_mat_spt*, respectively) weighted by their respective
particle number density given by *pnd_field*, for a single location
within the cloudbox, given by *scat_p_index*, *scat_lat_index*, and
*scat_lon_index*.
The resulting  extinction matrix is added to the workspace variable
*ext_mat*.

@author Jana Mendrok, Sreerekha T.R.

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void opt_prop_bulkCalc(Workspace& ws) {
  opt_prop_bulkCalc(Var::ext_mat(ws), Var::abs_vec(ws), Var::ext_mat_spt(ws),
                    Var::abs_vec_spt(ws), Var::pnd_field(ws),
                    Var::scat_p_index(ws), Var::scat_lat_index(ws),
                    Var::scat_lon_index(ws), Var::verbosity(ws));
}

/*! Calculates monochromatic optical properties for all scattering
elements.

In this function the extinction matrix and the absorption vector
are calculated in the laboratory frame. An interpolation of the
data on the actual frequency is the first step in this function.
The next step is a transformation from the database coordinate
system to the laboratory coordinate system.

Output of the function are *ext_mat_spt* and *abs_vec_spt*, which
hold the optical properties for a specified propagation direction
for each scattering element.

@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void opt_prop_sptFromData(Workspace& ws) {
  opt_prop_sptFromData(
      Var::ext_mat_spt(ws), Var::abs_vec_spt(ws), Var::scat_data(ws),
      Var::za_grid(ws), Var::aa_grid(ws), Var::za_index(ws), Var::aa_index(ws),
      Var::f_index(ws), Var::f_grid(ws), Var::rtp_temperature(ws),
      Var::pnd_field(ws), Var::scat_p_index(ws), Var::scat_lat_index(ws),
      Var::scat_lon_index(ws), Var::verbosity(ws));
}

/*! Calculates optical properties for the scattering elements.

As *opt_prop_sptFromData* but no frequency interpolation is
performed. The single scattering data is here obtained from
*scat_data_mono*, instead of *scat_data*.

@author Cory Davis

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void opt_prop_sptFromMonoData(Workspace& ws) {
  opt_prop_sptFromMonoData(
      Var::ext_mat_spt(ws), Var::abs_vec_spt(ws), Var::scat_data_mono(ws),
      Var::za_grid(ws), Var::aa_grid(ws), Var::za_index(ws), Var::aa_index(ws),
      Var::rtp_temperature(ws), Var::pnd_field(ws), Var::scat_p_index(ws),
      Var::scat_lat_index(ws), Var::scat_lon_index(ws), Var::verbosity(ws));
}

/*! Derives monochromatic optical properties for all scattering
elements.

As *opt_prop_sptFromData*, but using frequency pre-interpolated
data (as produced by *scat_dataCalc*), i.e. in here no frequency
interpolation is done anymore.

@author Jana Mendrok, Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void opt_prop_sptFromScat_data(Workspace& ws) {
  opt_prop_sptFromScat_data(
      Var::ext_mat_spt(ws), Var::abs_vec_spt(ws), Var::scat_data(ws),
      Var::scat_data_checked(ws), Var::za_grid(ws), Var::aa_grid(ws),
      Var::za_index(ws), Var::aa_index(ws), Var::f_index(ws),
      Var::rtp_temperature(ws), Var::pnd_field(ws), Var::scat_p_index(ws),
      Var::scat_lat_index(ws), Var::scat_lon_index(ws), Var::verbosity(ws));
}

/*! Sets the output file format to ASCII.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void output_file_formatSetAscii(Workspace& ws) {
  output_file_formatSetAscii(Var::output_file_format(ws), Var::verbosity(ws));
}

/*! Sets the output file format to binary.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void output_file_formatSetBinary(Workspace& ws) {
  output_file_formatSetBinary(Var::output_file_format(ws), Var::verbosity(ws));
}

/*! Sets the output file format to zipped ASCII.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void output_file_formatSetZippedAscii(Workspace& ws) {
  output_file_formatSetZippedAscii(Var::output_file_format(ws),
                                   Var::verbosity(ws));
}

/*! A simple way to make *p_grid* more dense.

The method includes new values in *p_grid*. For each intermediate
pressure range, *nfill* points are added. That is, setting *nfill*
to zero returns an unmodified copy of *p_grid_old*. The number of
elements of the new *p_grid* is (n0-1)*(1+nfill)+1, where n0 is the
length of *p_grid_old*.

The new points are distributed equidistant in log(p).

For safety, new grid and old grid Vectors are not allowed to be the
same variable (both will be needed later on for regridding of the
atmospheric fields), and atmospheric field related *checked WSV are
reset to 0 (unchecked).

@author Patrick Eriksson, Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] p_grid_old - A copy of the current (the old) p_grid. Not allowed to
be the same variable as the output *p_grid*.
@param[in] nfill - Number of points to add between adjacent pressure points.The
default value (-1) results in an error. (default: -1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void p_gridDensify(Workspace& ws, const Vector& p_grid_old,
                   const Index& nfill = -1) {
  p_gridDensify(Var::p_grid(ws), Var::atmfields_checked(ws),
                Var::atmgeom_checked(ws), Var::cloudbox_checked(ws), p_grid_old,
                nfill, Var::verbosity(ws));
}

/*! Sets *p_grid* to the pressure grid of *abs_lookup*.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void p_gridFromGasAbsLookup(Workspace& ws) {
  p_gridFromGasAbsLookup(Var::p_grid(ws), Var::abs_lookup(ws),
                         Var::verbosity(ws));
}

/*! Sets *p_grid* according to input atmosphere's raw z_field, derived
e.g. from *AtmRawRead*.
Attention: as default only pressure values for altitudes >= 0 are
extracted. If negative altitudes shall also be selected, set no_neg=0.

@author Claudia Emde, Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] no_negZ - Exclude negative altitudes. (default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void p_gridFromZRaw(Workspace& ws, const Index& no_negZ = 1) {
  p_gridFromZRaw(Var::p_grid(ws), Var::z_field_raw(ws), no_negZ,
                 Var::verbosity(ws));
}

/*! Provides refined pressure grid.

Created new pressure grid has (log10) spacings below a given
threshold.

For safety, new grid and old grid Vectors are not allowed to be the
same variable (both will be needed later on for regridding of the
atmospheric fields), and atmospheric field related *checked WSV are
reset to 0 (unchecked).

@author Stefan Buehler, Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] p_grid_old - A copy of the current (the old) p_grid. Not allowed to
be the same variable as the output *p_grid*.
@param[in] p_step - Maximum step in log10(p[Pa]). If the pressure grid is
coarser than this, additional points are added until each log step is smaller
than this.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void p_gridRefine(Workspace& ws, const Vector& p_grid_old,
                  const Numeric& p_step) {
  p_gridRefine(Var::p_grid(ws), Var::atmfields_checked(ws),
               Var::atmgeom_checked(ws), Var::cloudbox_checked(ws), p_grid_old,
               p_step, Var::verbosity(ws));
}

/*! Clipping of *particle_bulkprop_field*.

The method allows you to apply hard limits the values of
*particle_bulkprop_field*. All values, of the property selected,
below *limit_low*, are simply set to *limit_low*. And the same
is performed with respect to *limit_high*. That is, the data in x
for the retrieval quantity are forced to be inside the range
[limit_low,limit_high].

Setting species="ALL", is a shortcut for applying the limits on all
properties.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] bulkprop_name - Name of bulk property to consider, or "ALL".
@param[in] limit_low - Lower limit for clipping. (default:
-std::numeric_limits<Numeric>::infinity())
@param[in] limit_high - Upper limit for clipping. (default:
std::numeric_limits<Numeric>::infinity())

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void particle_bulkprop_fieldClip(
    Workspace& ws, const String& bulkprop_name,
    const Numeric& limit_low = -std::numeric_limits<Numeric>::infinity(),
    const Numeric& limit_high = std::numeric_limits<Numeric>::infinity()) {
  particle_bulkprop_fieldClip(Var::particle_bulkprop_field(ws),
                              Var::particle_bulkprop_names(ws), bulkprop_name,
                              limit_low, limit_high, Var::verbosity(ws));
}

/*! Adds a perturbation to *particle_bulkprop_field*.

Works as *AtmFieldPerturb* but acts on *particle_bulkprop_field*.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] particle_type - Name of field to perturb, such as IWC.
@param[in] p_ret_grid - Pressure retrieval grid.
@param[in] lat_ret_grid - Latitude retrieval grid.
@param[in] lon_ret_grid - Longitude retrieval grid.
@param[in] pert_index - Index of position where the perturbation shall be
performed.
@param[in] pert_size - Size of perturbation.
@param[in] pert_mode - Type of perturbation, ansolute or relative. (default:
"absolute")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void particle_bulkprop_fieldPerturb(Workspace& ws, const String& particle_type,
                                    const Vector& p_ret_grid,
                                    const Vector& lat_ret_grid,
                                    const Vector& lon_ret_grid,
                                    const Index& pert_index,
                                    const Numeric& pert_size,
                                    const String& pert_mode = "absolute") {
  particle_bulkprop_fieldPerturb(
      Var::particle_bulkprop_field(ws), Var::atmosphere_dim(ws),
      Var::p_grid(ws), Var::lat_grid(ws), Var::lon_grid(ws),
      Var::particle_bulkprop_names(ws), particle_type, p_ret_grid, lat_ret_grid,
      lon_ret_grid, pert_index, pert_size, pert_mode, Var::verbosity(ws));
}

/*! Adds a perturbation to *particle_bulkprop_field*.

Works as *AtmFieldPerturbAtmGrids* but acts on *particle_bulkprop_field*.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] particle_type - Name of field to perturb, such as IWC.
@param[in] pert_index - Index of position where the perturbation shall be
performed.
@param[in] pert_size - Size of perturbation.
@param[in] pert_mode - Type of perturbation, ansolute or relative. (default:
"absolute")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void particle_bulkprop_fieldPerturbAtmGrids(
    Workspace& ws, const String& particle_type, const Index& pert_index,
    const Numeric& pert_size, const String& pert_mode = "absolute") {
  particle_bulkprop_fieldPerturbAtmGrids(
      Var::particle_bulkprop_field(ws), Var::atmosphere_dim(ws),
      Var::p_grid(ws), Var::lat_grid(ws), Var::lon_grid(ws),
      Var::particle_bulkprop_names(ws), particle_type, pert_index, pert_size,
      pert_mode, Var::verbosity(ws));
}

/*! Removes unrealistically small or erroneous data from particle fields.

This WSM checks if the input particle field (e.g.
*particle_bulkprop_field*, scat_species_XXX_field) contains values
smaller than the given *threshold*. In this case, these values will
be set to zero.

The method should be applied if the particle fields contain
unrealistically small or erroneous data (NWP/GCM model data, e.g.
from the Chevallierl_91l sets, often contain very small or even
negative values, which are numerical artefacts rather than physical
values.)
For the scat_species_XXX_fields, it needs to be applied separately
per Tensor4 type field collection. This allows to use different
thresholds for the different types of fields (not for the different
scattering species, though).

*particle_fieldCleanup* shall be called after generation of the
atmopheric fields.

@author Daniel Kreyling

@param[in,out] Workspace ws - An ARTS workspace
@param[out] particle_field_out - A particle property field, e.g.
*particle_bulkprop_field* or *scat_species_mass_density_field*
@param[in] particle_field_in - A particle property field, e.g.
*particle_bulkprop_field* or *scat_species_mass_density_field*
@param[in] threshold - Threshold below which the *particle_field* values are set
to zero.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void particle_fieldCleanup(Workspace& ws, Tensor4& particle_field_out,
                           const Tensor4& particle_field_in,
                           const Numeric& threshold) {
  particle_fieldCleanup(particle_field_out, particle_field_in, threshold,
                        Var::verbosity(ws));
}

/*! Derives *particle_masses* from *scat_meta*.

This method is supposed to be used to derive *particle_masses*
when *pnd_field* is internally calculated using
*pnd_fieldCalcFromParticleBulkProps* (in contrast to reading it
from external sources using *ScatElementsPndAndScatAdd* and
*pnd_fieldCalcFrompnd_field_raw*).
It extracts the mass information of the scattering elements from
*scat_meta*. Each scattering species is taken as a separate
category of particle_masses, i.e., the resulting
*particle_masses* matrix will contain as many columns as
scattering species are present in *scat_meta*.

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void particle_massesFromMetaData(Workspace& ws) {
  particle_massesFromMetaData(Var::particle_masses(ws), Var::scat_meta(ws),
                              Var::verbosity(ws));
}

/*! Sets *particle_masses* based on *scat_meta* assuming
all particles are of the same mass category.

This method derives the particle masses from the mass entry
of each scattering element. It is assumed that all scattering
elements represent particles of the same (bulk) matter
(e.g. water or ice). With other words, a single mass category
is assumed (see *particle_masses* for a definition of "mass
category").

To be clear, the above are assumptions of the method, the user
is free to work with any scattering element. For Earth and just having
cloud and particles, the resulting mass category can be seen as
the total cloud water content, with possible contribution from
both ice and liquid phase.

@author Jana Mendrok
@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void particle_massesFromMetaDataSingleCategory(Workspace& ws) {
  particle_massesFromMetaDataSingleCategory(
      Var::particle_masses(ws), Var::scat_meta(ws), Var::verbosity(ws));
}

/*! Initialize partition functions with default values from built-in
species data.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void partition_functionsInitFromBuiltin(Workspace& ws) {
  partition_functionsInitFromBuiltin(Var::partition_functions(ws),
                                     Var::verbosity(ws));
}

/*! Calculates the total phase matrix of all scattering elements.

This function sums up the monochromatic phase matrices of all
scattering elements *pha_mat_spt* weighted with  their respective
particle number density, given by *pnd_field*, for a single location
within the cloudbox, given by *scat_p_index*, *scat_lat_index*, and
*scat_lon_index*.

@author Sreerekha T.R.

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void pha_matCalc(Workspace& ws) {
  pha_matCalc(Var::pha_mat(ws), Var::pha_mat_spt(ws), Var::pnd_field(ws),
              Var::atmosphere_dim(ws), Var::scat_p_index(ws),
              Var::scat_lat_index(ws), Var::scat_lon_index(ws),
              Var::verbosity(ws));
}

/*! Calculation of the phase matrix of the individual scattering elements.

This function can be used in *pha_mat_spt_agenda* as part of
the calculation of the scattering integral.

First, data at the requested frequency (given by *f_grid* and
*f_index*) and temperature (given by *rtp_temperature*) is
extracted. This is followed by a transformation from the database
coordinate system to the laboratory coordinate system.

Frequency extraction is always done by (linear) interpolation.
Temperature is (linearly) interpolated when at least two
temperature grid points are present in the *scat_data* and
*rtp_temperature* is positive. If only a single temperature point
is available, data for this point is used without modification. In
order to speed up calculations, temperature interpolation can be
avoided by passing a *rtp_temperature*<0. In this case, a specific
temperature grid from the *scat_data* grid is used without
modification. The selection is as follows:
  -10 < *rtp_temperature * <   0   T_grid[0]     lowest temperature
  -20 < *rtp_temperature * < -10   T_grid[nT-1]  highest temperature
        *rtp_temperature*  < -20   T_grid[nT/2]  median grid point

@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void pha_mat_sptFromData(Workspace& ws) {
  pha_mat_sptFromData(Var::pha_mat_spt(ws), Var::scat_data(ws),
                      Var::za_grid(ws), Var::aa_grid(ws), Var::za_index(ws),
                      Var::aa_index(ws), Var::f_index(ws), Var::f_grid(ws),
                      Var::rtp_temperature(ws), Var::pnd_field(ws),
                      Var::scat_p_index(ws), Var::scat_lat_index(ws),
                      Var::scat_lon_index(ws), Var::verbosity(ws));
}

/*! Calculation of the phase matrix of the individual scattering elements.

In this function the phase matrix is extracted from
*pha_mat_sptDOITOpt*. It can be used in the agenda
*pha_mat_spt_agenda*. This method must be used in combination with
*DoitScatteringDataPrepare*.

Temperature is considered as described for *pha_mat_sptFromData*

@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void pha_mat_sptFromDataDOITOpt(Workspace& ws) {
  pha_mat_sptFromDataDOITOpt(
      Var::pha_mat_spt(ws), Var::pha_mat_sptDOITOpt(ws),
      Var::scat_data_mono(ws), Var::doit_za_grid_size(ws), Var::aa_grid(ws),
      Var::za_index(ws), Var::aa_index(ws), Var::rtp_temperature(ws),
      Var::pnd_field(ws), Var::scat_p_index(ws), Var::scat_lat_index(ws),
      Var::scat_lon_index(ws), Var::verbosity(ws));
}

/*! Calculation of the phase matrix of the individual scattering elements.

This function is the monochromatic version of *pha_mat_sptFromData*.

@author Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void pha_mat_sptFromMonoData(Workspace& ws) {
  pha_mat_sptFromMonoData(
      Var::pha_mat_spt(ws), Var::scat_data_mono(ws), Var::doit_za_grid_size(ws),
      Var::aa_grid(ws), Var::za_index(ws), Var::aa_index(ws),
      Var::rtp_temperature(ws), Var::pnd_field(ws), Var::scat_p_index(ws),
      Var::scat_lat_index(ws), Var::scat_lon_index(ws), Var::verbosity(ws));
}

/*! Calculation of the phase matrix of the individual scattering elements.

As *pha_mat_sptFromData*, but using frequency pre-interpolated
data (as produced by *scat_dataCalc*), i.e. in here no frequency
interpolation is done anymore.

@author Jana Mendrok, Claudia Emde

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void pha_mat_sptFromScat_data(Workspace& ws) {
  pha_mat_sptFromScat_data(
      Var::pha_mat_spt(ws), Var::scat_data(ws), Var::scat_data_checked(ws),
      Var::za_grid(ws), Var::aa_grid(ws), Var::za_index(ws), Var::aa_index(ws),
      Var::f_index(ws), Var::rtp_temperature(ws), Var::pnd_field(ws),
      Var::scat_p_index(ws), Var::scat_lat_index(ws), Var::scat_lon_index(ws),
      Var::verbosity(ws));
}

/*! Calculates *pnd_data* from given *psd_data* for one scattering species.

Performs integration of the size distribution over the size grid
bin deriving pnd (units #/m3) from psd (units #/m3/m). Some checks
on the sufficiency of the size grid range and coverage are applied.

*quad_order* can be 0 for rectangular or 1 for trapezoidal
integration. The only difference is the treatment of the start and
end nodes. For trapezoidal their corresponding bins end exactly at
the nodes, while for rectangular they extend further out by the half
distance to the neighbor node (but not beyond 0).

Attempts to check that the size grids and *scat_data* represent the
bulk extinction sufficiently. Specifically, it is tested that
 (a) psd*ext is decreasing at the small and large particle size
     ends of the size grid - but only if scattering species bulk
     extinction exceeds 1% of *threshold_ss_ext*.
 (b) removing the smallest and largest particles changes the
     resulting bulk extinction by less then a fraction of
     *threshold_se_ext* - but only if scattering species bulk
     extinction exceeds *threshold_ss_ext* and number density (pnd)
     of the edge size point at this atmospheric level is larger
     than *threshold_se_pnd* times the maximum pnd of this
     scattering element over all atmospheric levels.
Skipping tests in case of low extinction is done in order to
minimize issues arising from very low mass densities,
particularly at single atmospheric levels, and very low bulk
extinctions, i.e. in cases where the effects on the radiance fields
are estimated to be low.
NOTE: The tests are only approximate and do not guarantee the
validity of the resulting bulk properties (and increasing the
thresholds will decrease the reliability of the bulk properties).

@author Jana Mendrok, Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] quad_order - Order of bin quadrature. (default: 1)
@param[in] scat_index - Take data from scattering species of this index
(0-based) in *scat_data*.
@param[in] threshold_se_ext - Maximum allowed extinction fraction in each of the
edge size bins. (default: 0.02)
@param[in] threshold_ss_ext - Minimum bulk extinction in the processed
scattering species for which to apply size grid representation checks. (default:
1e-8)
@param[in] threshold_se_pnd - Minimum ratio of edge point pnd to maximum pnd of
this scattering element over all pressure levels. (default: 0.02)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void pndFromPsd(Workspace& ws, const Index& scat_index,
                const Index& quad_order = 1,
                const Numeric& threshold_se_ext = 0.02,
                const Numeric& threshold_ss_ext = 1e-8,
                const Numeric& threshold_se_pnd = 0.02) {
  pndFromPsd(Var::pnd_data(ws), Var::dpnd_data_dx(ws), Var::pnd_size_grid(ws),
             Var::psd_data(ws), Var::psd_size_grid(ws), Var::dpsd_data_dx(ws),
             Var::scat_data(ws), Var::f_grid(ws), Var::scat_data_checked(ws),
             quad_order, scat_index, threshold_se_ext, threshold_ss_ext,
             threshold_se_pnd, Var::verbosity(ws));
}

/*! Calculates *pnd_data* from given *psd_data*.

As *pndFromPsdBasic*, but without bulk extinction representation
checks.

@author Jana Mendrok, Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] quad_order - Order of bin quadrature. (default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void pndFromPsdBasic(Workspace& ws, const Index& quad_order = 1) {
  pndFromPsdBasic(Var::pnd_data(ws), Var::dpnd_data_dx(ws),
                  Var::pnd_size_grid(ws), Var::psd_data(ws),
                  Var::psd_size_grid(ws), Var::dpsd_data_dx(ws), quad_order,
                  Var::verbosity(ws));
}

/*! Converts particle bulk property data to *pnd_field*.

In short, the method combines *scat_species*, *pnd_agenda_array*,
*particle_bulkprop_field* and their associated variables to derive
*pnd_field*.

The method does nothing if cloudbox is inactive.

Otherwise, cloudbox limits must be set before calling the method,
and *particle_bulkprop_field* is checked to have non-zero elements
just inside the cloudbox.

@author Patrick Eriksson, Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void pnd_fieldCalcFromParticleBulkProps(Workspace& ws) {
  pnd_fieldCalcFromParticleBulkProps(
      ws, Var::pnd_field(ws), Var::dpnd_field_dx(ws), Var::atmosphere_dim(ws),
      Var::p_grid(ws), Var::lat_grid(ws), Var::lon_grid(ws), Var::t_field(ws),
      Var::cloudbox_on(ws), Var::cloudbox_limits(ws), Var::scat_species(ws),
      Var::scat_data(ws), Var::scat_meta(ws), Var::particle_bulkprop_field(ws),
      Var::particle_bulkprop_names(ws), Var::pnd_agenda_array(ws),
      Var::pnd_agenda_array_input_names(ws), Var::jacobian_do(ws),
      Var::jacobian_quantities(ws), Var::verbosity(ws));
}

/*! Interpolation of particle number density fields to calculation grid
inside cloudbox.

This method interpolates the particle number density field
from the raw data *pnd_field_raw* to obtain *pnd_field*.
For 1D cases, where internally *GriddedFieldPRegrid* and
*GriddedFieldLatLonRegrid* are applied, *zeropadding*=1 sets the
*pnd_field* at pressure levels levels exceeding pnd_field_raw's
pressure grid to 0 (not implemented for 2D and 3D yet). Default:
zeropadding=0, which throws an error if the calculation pressure grid
*p_grid* is not completely covered by pnd_field_raw's pressure grid.

@author Sreerekha T.R.
@author Claudia Emde
@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] zeropadding - Allow zeropadding of pnd_field. (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void pnd_fieldCalcFrompnd_field_raw(Workspace& ws,
                                    const Index& zeropadding = 0) {
  pnd_fieldCalcFrompnd_field_raw(
      Var::pnd_field(ws), Var::dpnd_field_dx(ws), Var::p_grid(ws),
      Var::lat_grid(ws), Var::lon_grid(ws), Var::pnd_field_raw(ws),
      Var::atmosphere_dim(ws), Var::cloudbox_limits(ws),
      Var::jacobian_quantities(ws), zeropadding, Var::verbosity(ws));
}

/*! Maps a 1D pnd_field to a (homogeneous) 2D or 3D pnd_field.

This method takes a 1D *pnd_field* and converts it to a 2D or 3D
"cloud". It is assumed that a complete 1D case has been created,
and after this *atmosphere_dim*, *lat_grid*, *lon_grid* and
*cloudbox_limits* have been changed to a 2D or 3D case (without
changing the vertical extent of the cloudbox.

No modification of *pnd_field* is made for the pressure dimension.
At the latitude and longitude cloudbox edge points *pnd_field* is set to
zero. This corresponds to nzero=1. If you want a larger margin between
the lat and lon cloudbox edges and the "cloud" you increase
*nzero*, where *nzero* is the number of grid points for which
*pnd_field* shall be set to 0, counted from each lat and lon edge.

See further *AtmFieldsExpand1D*.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] nzero - Number of zero values inside lat and lon limits. (default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void pnd_fieldExpand1D(Workspace& ws, const Index& nzero = 1) {
  pnd_fieldExpand1D(Var::pnd_field(ws), Var::atmosphere_dim(ws),
                    Var::cloudbox_on(ws), Var::cloudbox_limits(ws), nzero,
                    Var::verbosity(ws));
}

/*! Sets *pnd_field* to zero.

Creates an empty *pnd_field* of cloudbox size according to
*cloudbox_limits* and with number of scattering elemements
according to *scat_data*. If *scat_data* is not set yet, it will be
filled with one dummy scattering element.

The method works with both *scat_data* and *scat_data_raw*.
This method primarily exists for testing purposes.
On the one hand, empty *pnd_field* runs can be used to test the
agreement between true clear-sky (*cloudboxOff*) solutions and the
scattering solver solution in factual clear-sky conditions. It is
important to avoid discontinuities when switching from thin-cloud
to clear-sky conditions.
Moreover, scattering calculations using the DOIT method include
interpolation errors. If one is interested in this effect, one
should compare the DOIT result with an empty cloudbox to a clearsky
calculation. That means that the iterative method is performed for
a cloudbox with no particles.

@author Claudia Emde, Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void pnd_fieldZero(Workspace& ws) {
  pnd_fieldZero(Var::pnd_field(ws), Var::dpnd_field_dx(ws), Var::scat_data(ws),
                Var::atmosphere_dim(ws), Var::f_grid(ws),
                Var::cloudbox_limits(ws), Var::jacobian_quantities(ws),
                Var::verbosity(ws));
}

/*! Stand-alone calculation of propagation paths.

Beside a few checks of input data, the only operation of this
method is to execute *ppath_agenda*.

Propagation paths are normally calculated as part of the radiative
transfer calculations, and this method is not part of the control
file. A reason to call this function directly would be to obtain a
propagation path for plotting. Anyhow, use this method instead
of calling e.g.*ppathStepByStep directly.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ppathCalc(Workspace& ws) {
  ppathCalc(ws, Var::ppath(ws), Var::ppath_agenda(ws), Var::ppath_lmax(ws),
            Var::ppath_lraytrace(ws), Var::atmgeom_checked(ws), Var::f_grid(ws),
            Var::cloudbox_on(ws), Var::cloudbox_checked(ws),
            Var::ppath_inside_cloudbox_do(ws), Var::rte_pos(ws),
            Var::rte_los(ws), Var::rte_pos2(ws), Var::verbosity(ws));
}

/*! Moves *rte_pos* forwards to near altitude before calling *ppathCalc*
to compute a different *ppath*.  The accuracy-variable gives minimum
distance before the input altitude.

The forward-moving algorithm calls *ppathCalc* several
times at reduced maximum distances.  The intention is to maintain
the correct *rte_los* for a given *rte_pos* at all altitudes.  The
method is thus relatively slow, and VERY memory intense at low
accuracy.

Intended to be used with "tropospheric corrections" from ground
geometry.  Not well-tested

Throws error if no altitude is in line of sight.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] altitude - Altitude to move forward towards
@param[in] accuracy - Accuracy of altitude (default: 0.5)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ppathCalcFromAltitude(Workspace& ws, const Numeric& altitude,
                           const Numeric& accuracy = 0.5) {
  ppathCalcFromAltitude(
      ws, Var::ppath(ws), Var::ppath_agenda(ws), Var::ppath_lmax(ws),
      Var::ppath_lraytrace(ws), Var::atmgeom_checked(ws), Var::f_grid(ws),
      Var::cloudbox_on(ws), Var::cloudbox_checked(ws),
      Var::ppath_inside_cloudbox_do(ws), Var::rte_pos(ws), Var::rte_los(ws),
      Var::rte_pos2(ws), altitude, accuracy, Var::verbosity(ws));
}

/*! Determines the propagation path from *rte_pos2* to *rte_pos*.

The propagation path linking *rte_pos* and *rte_pos2* is calculated
and returned. The method determines the path in a pure numerical
manner, where a simple algorithm is applied. The task is to find
the value of *rte_los* (at *rte_pos*) linking the two positions.

See the user guide for a description of the search algorithm,
including a more detailed definition of *za_accuracy*,
*pplrt_factor* and *pplrt_lowest*.

The standard application of this method should be to radio link
calculations, where *rte_pos2* corresponds to a transmitter, and
*rte_pos* to the receiver/sensor.

The details of the ray tracing is controlled by *ppath_step_agenda*
as usual.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] za_accuracy - Required accuracy, in form of the maximum allowed
angular off-set [deg]. (default: 2e-5)
@param[in] pplrt_factor - The factor with which ppath_lraytrace is decreased if
no solution is found. (default: 5)
@param[in] pplrt_lowest - Lowest value ppath_lraytrace to consider. The
calculations are halted if this length is passed. (default: 0.5)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ppathFromRtePos2(Workspace& ws, const Numeric& za_accuracy = 2e-5,
                      const Numeric& pplrt_factor = 5,
                      const Numeric& pplrt_lowest = 0.5) {
  ppathFromRtePos2(ws, Var::ppath(ws), Var::rte_los(ws),
                   Var::ppath_lraytrace(ws), Var::ppath_step_agenda(ws),
                   Var::atmosphere_dim(ws), Var::p_grid(ws), Var::lat_grid(ws),
                   Var::lon_grid(ws), Var::z_field(ws), Var::f_grid(ws),
                   Var::refellipsoid(ws), Var::z_surface(ws), Var::rte_pos(ws),
                   Var::rte_pos2(ws), Var::ppath_lmax(ws), za_accuracy,
                   pplrt_factor, pplrt_lowest, Var::verbosity(ws));
}

/*! Propagation path calculations for a plane parallel atmosphere.

This method basically assumes that the planet's radius is infinite,
i.e. the planet surface has no curvature. Some consequences of this
assumption:
   - the mathod can only be used for 1D
   - zenith angles between 89.9 and 90.1 deg are not allowed
   - refraction is always neglected
   - radii in ppath are set to Inf

Notice that the method provides full propagation paths. This means
that *ppath_step_agenda* is ignored (and thus also refraction).
On the other hand, the method considers the cloudbox exactly as
the standard path calculations.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ppathPlaneParallel(Workspace& ws) {
  ppathPlaneParallel(Var::ppath(ws), Var::atmosphere_dim(ws), Var::z_field(ws),
                     Var::z_surface(ws), Var::cloudbox_on(ws),
                     Var::cloudbox_limits(ws),
                     Var::ppath_inside_cloudbox_do(ws), Var::rte_pos(ws),
                     Var::rte_los(ws), Var::ppath_lmax(ws), Var::verbosity(ws));
}

/*! Standard method for calculation of propagation paths.

This method calculates complete propagation paths in a stepwise
manner. Each step is denoted as a "ppath_step" and is the path
through/inside a single grid box.

The definition of a propgation path cannot be accommodated here.
For more information read the chapter on propagation paths in the
ARTS user guide.

This method should never be called directly. Use *ppathCalc* instead
if you want to extract propagation paths.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ppathStepByStep(Workspace& ws) {
  ppathStepByStep(ws, Var::ppath(ws), Var::ppath_step_agenda(ws),
                  Var::ppath_inside_cloudbox_do(ws), Var::atmosphere_dim(ws),
                  Var::p_grid(ws), Var::lat_grid(ws), Var::lon_grid(ws),
                  Var::z_field(ws), Var::f_grid(ws), Var::refellipsoid(ws),
                  Var::z_surface(ws), Var::cloudbox_on(ws),
                  Var::cloudbox_limits(ws), Var::rte_pos(ws), Var::rte_los(ws),
                  Var::ppath_lmax(ws), Var::ppath_lraytrace(ws),
                  Var::verbosity(ws));
}

/*! WSM to only write a reduced Ppath, omitting grid positions.

The following fields are set to be empty: gp_p, gp_lat and gp_lon.
This cam drastically decrease the time for reading the structure
by some external software.

If *file_index is >= 0, the variable is written to a file with name:
   <filename>.<file_index>.xml.
where <file_index> is the value of *file_index*.

This means that *filename* shall here not include the .xml
extension. Omitting filename works as for *WriteXML*.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] filename - File name. See above. (default: "")
@param[in] file_index - Optional file index to append to filename. (default: -1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ppathWriteXMLPartial(Workspace& ws, const String& filename = "",
                          const Index& file_index = -1) {
  ppathWriteXMLPartial(Var::output_file_format(ws), Var::ppath(ws), filename,
                       file_index, Var::verbosity(ws));
}

/*! Stand-alone calculation of propagation path field from sensors.

Uses *ppathCalc* internally.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ppath_fieldCalc(Workspace& ws) {
  ppath_fieldCalc(ws, Var::ppath_field(ws), Var::ppath_agenda(ws),
                  Var::ppath_lmax(ws), Var::ppath_lraytrace(ws),
                  Var::atmgeom_checked(ws), Var::f_grid(ws),
                  Var::cloudbox_on(ws), Var::cloudbox_checked(ws),
                  Var::ppath_inside_cloudbox_do(ws), Var::sensor_pos(ws),
                  Var::sensor_los(ws), Var::rte_pos2(ws), Var::verbosity(ws));
}

/*! Computes ppath_field from "standalone" sensors looking upwards from
0 m altitude with zenith angles range [0, 90], downwards from the top
of the atmosphere covering the zenith angle range from 180 degrees to
the surface tangent minus 1e-4 degrees, and through the limb covering
at the same position as the downwards looking sensor covering the zenith
angle range from the surface tangent plus 1e-4 degrees to 90 degrees minus
1e-4 degrees.

The top of the atmosphere is from *z_field*(-1, 0, 0) [python range notation].

The field will consist of 3*nz arrays structured as [up, limb, down]

The intent of this function is to generate a field so that calculations
of *ppvar_iy* of all the fields will cover the zenith angle space
of all positions in *z_field*.

Only works for *atmosphere_dim* 1, spherical planets, and *ppath_lmax*<0

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] nz - Number of zenith angles per position (default: 3)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ppath_fieldFromDownUpLimbGeoms(Workspace& ws, const Index& nz = 3) {
  ppath_fieldFromDownUpLimbGeoms(
      ws, Var::ppath_field(ws), Var::ppath_agenda(ws), Var::ppath_lmax(ws),
      Var::ppath_lraytrace(ws), Var::atmgeom_checked(ws), Var::z_field(ws),
      Var::f_grid(ws), Var::cloudbox_on(ws), Var::cloudbox_checked(ws),
      Var::ppath_inside_cloudbox_do(ws), Var::rte_pos(ws), Var::rte_los(ws),
      Var::rte_pos2(ws), Var::refellipsoid(ws), Var::atmosphere_dim(ws), nz,
      Var::verbosity(ws));
}

/*! Calculates a geometrical propagation path step.

This function determines a propagation path step by pure
geometrical calculations. That is, refraction is neglected. Path
points are always included for crossings with the grids, tangent
points and intersection points with the surface. The WSV *ppath_lmax*
gives the option to include additional points to ensure that the
distance along the path between the points does not exceed the
selected maximum length. No additional points are included if
*ppath_lmax* is set to <= 0.

For further information, type see the on-line information for
*ppath_step_agenda*.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ppath_stepGeometric(Workspace& ws) {
  ppath_stepGeometric(Var::ppath_step(ws), Var::atmosphere_dim(ws),
                      Var::lat_grid(ws), Var::lon_grid(ws), Var::z_field(ws),
                      Var::refellipsoid(ws), Var::z_surface(ws),
                      Var::ppath_lmax(ws), Var::verbosity(ws));
}

/*! Calculates a propagation path step, considering refraction by a
basic approach.

Refraction is taken into account by probably the simplest approach
possible. The path is treated to consist of piece-wise geometric
steps. A geometric path step is calculated from each point by
using the local line-of-sight. Snell's law for spherical symmetry
is used for 1D to determine the zenith angle at the new point.
For 2D and 3D, the zenith angle is calculated using the average
gradient of the refractive index between the two points. For 3D,
the azimuth angle is treated in the same way as the zenith one.

The maximum length of each ray tracing step is given by the WSV
*ppath_lraytrace*. The length will never exceed the given maximum,
but it can be smaller. The ray tracing steps are only used to
determine the path. Points to describe the path are included as
for *ppath_stepGeometric*, this including the functionality of
*ppath_lmax*.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ppath_stepRefractionBasic(Workspace& ws) {
  ppath_stepRefractionBasic(
      ws, Var::ppath_step(ws), Var::refr_index_air_agenda(ws),
      Var::atmosphere_dim(ws), Var::p_grid(ws), Var::lat_grid(ws),
      Var::lon_grid(ws), Var::z_field(ws), Var::t_field(ws), Var::vmr_field(ws),
      Var::refellipsoid(ws), Var::z_surface(ws), Var::f_grid(ws),
      Var::ppath_lmax(ws), Var::ppath_lraytrace(ws), Var::verbosity(ws));
}

/*! Sets *ppvar_optical_depth* according to provided transmission data.

The values in ppvar_optical_depth are set to
-log( ppvar_trans_cumulat(joker,joker,0,0) ).

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ppvar_optical_depthFromPpvar_trans_cumulat(Workspace& ws) {
  ppvar_optical_depthFromPpvar_trans_cumulat(Var::ppvar_optical_depth(ws),
                                             Var::ppvar_trans_cumulat(ws),
                                             Var::verbosity(ws));
}

/*! Calculates absorption matrix describing Faraday rotation.

Faraday rotation is a change of polarization state of an
electromagnetic wave propagating through charged matter by
interaction with a magnetic field. Hence, this method requires
*abs_species* to contain 'free_electrons' and electron content field
(as part of *vmr_field*) as well as magnetic field (*mag_u_field*,
*mag_v_field*, *mag_w_field*) to be specified.

Faraday rotation affects Stokes parameters 2 and 3 (but not
intensity!). Therefore, this method requires stokes_dim>2.

Like all 'propmat_clearskyAdd*' methods, the method is additive,
i.e., does not overwrite the propagation matrix *propmat_clearsky*,
but adds further contributions.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void propmat_clearskyAddFaraday(Workspace& ws) {
  propmat_clearskyAddFaraday(
      Var::propmat_clearsky(ws), Var::dpropmat_clearsky_dx(ws),
      Var::stokes_dim(ws), Var::atmosphere_dim(ws), Var::f_grid(ws),
      Var::abs_species(ws), Var::jacobian_quantities(ws), Var::rtp_vmr(ws),
      Var::rtp_los(ws), Var::rtp_mag(ws), Var::verbosity(ws));
}

/*! Copy *propmat_clearsky* from *abs_coef_per_species*. This is handy for
putting an explicit line-by-line calculation into the *propmat_clearsky_agenda*.
This method is also used internally by. *propmat_clearskyAddOnTheFly*. Like all
other propmat_clearsky methods, this method does not overwrite prior content of
*propmat_clearsky*, but adds to it.

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void propmat_clearskyAddFromAbsCoefPerSpecies(Workspace& ws) {
  propmat_clearskyAddFromAbsCoefPerSpecies(
      Var::propmat_clearsky(ws), Var::dpropmat_clearsky_dx(ws),
      Var::abs_coef_per_species(ws), Var::dabs_coef_dx(ws), Var::verbosity(ws));
}

/*! Extract gas absorption coefficients from lookup table.

This extracts the absorption coefficient for all species from the
lookup table, and adds them to the propagation matrix. Extraction is
for one specific atmospheric condition, i.e., a set of pressure,
temperature, and VMR values.

Some special species are ignored, for example Zeeman species and free
electrons, since their absorption properties are not simple scalars
and cannot be handled by the lookup table.

The interpolation order in T and H2O is given by *abs_t_interp_order*
and *abs_nls_interp_order*, respectively.

Extraction is done for the frequencies in f_grid. Frequency
interpolation is controlled by *abs_f_interp_order*. If this is zero,
then f_grid must either be the same as the internal frequency grid of
the lookup table (for efficiency reasons, only the first and last
element of f_grid are checked), or must have only a single element.
If *abs_f_interp_order* is above zero, then frequency is interpolated
along with the other interpolation dimensions. This is useful for
calculations with Doppler shift.

For Doppler calculations, you should generate the table with a
somewhat larger frequency grid than the calculation itself has, since
the Doppler shift will push the frequency grid out of the table range
on one side.

Some extrapolation is allowed. For pressure and frequency interpolation
the standard extrapolation factor of 0.5 is applied. The factor is the
default for temperature and VMR interpolation, but the extrapolation
limit can here be adjusted by the *extpolfac* argument.

See also: *propmat_clearskyAddOnTheFly*.

@author Stefan Buehler, Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] extpolfac - Extrapolation factor (for temperature and VMR grid
edges). (default: 0.5)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void propmat_clearskyAddFromLookup(Workspace& ws,
                                   const Numeric& extpolfac = 0.5) {
  propmat_clearskyAddFromLookup(
      Var::propmat_clearsky(ws), Var::dpropmat_clearsky_dx(ws),
      Var::abs_lookup(ws), Var::abs_lookup_is_adapted(ws),
      Var::abs_p_interp_order(ws), Var::abs_t_interp_order(ws),
      Var::abs_nls_interp_order(ws), Var::abs_f_interp_order(ws),
      Var::f_grid(ws), Var::rtp_pressure(ws), Var::rtp_temperature(ws),
      Var::rtp_vmr(ws), Var::jacobian_quantities(ws), extpolfac,
      Var::verbosity(ws));
}

/*! Calculates gas absorption coefficients line-by-line for HITRAN line mixed
data.

*Wigner6Init* or *Wigner3Init* must be called before this function.


Please ensure you cite the original authors when you use this function:
        J. Lamouroux, L. Realia, X. Thomas, et al., J.Q.S.R.T. 151 (2015), 88-96

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void propmat_clearskyAddHitranLineMixingLines(Workspace& ws) {
  propmat_clearskyAddHitranLineMixingLines(
      Var::propmat_clearsky(ws), Var::abs_hitran_relmat_data(ws),
      Var::abs_lines_per_species(ws), Var::f_grid(ws), Var::abs_species(ws),
      Var::jacobian_quantities(ws), Var::partition_functions(ws),
      Var::rtp_pressure(ws), Var::rtp_temperature(ws), Var::rtp_vmr(ws),
      Var::verbosity(ws));
}

/*! Calculates gas absorption coefficients line-by-line.

This method can be used inside *propmat_clearsky_agenda* just like
*propmat_clearskyAddFromLookup*. It is a shortcut for putting in some
other methods explicitly, namely:

  1. *AbsInputFromRteScalars*
  2. Execute *abs_xsec_agenda*
  3. *abs_coefCalcFromXsec*
  4. *propmat_clearskyAddFromAbsCoefPerSpecies*

The calculation is for one specific atmospheric condition, i.e., a set
of pressure, temperature, and VMR values.

@author Stefan Buehler, Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void propmat_clearskyAddOnTheFly(Workspace& ws) {
  propmat_clearskyAddOnTheFly(
      ws, Var::propmat_clearsky(ws), Var::nlte_source(ws),
      Var::dpropmat_clearsky_dx(ws), Var::dnlte_dx_source(ws),
      Var::nlte_dsource_dx(ws), Var::f_grid(ws), Var::abs_species(ws),
      Var::jacobian_quantities(ws), Var::rtp_pressure(ws),
      Var::rtp_temperature(ws), Var::rtp_nlte(ws), Var::rtp_vmr(ws),
      Var::abs_xsec_agenda(ws), Var::verbosity(ws));
}

/*! Calculates absorption coefficients of particles to be used in
clearsky (non-cloudbox) calculations.

This is a method to include particles (neglecting possible
scattering components) in a clearsky calculation, i.e. without
applying the cloudbox and scattering solvers. Particles are handled
as absorbing species with one instance of 'particles' per scattering
element considered added to *abs_species*. Particle absorption cross-
sections at current atmospheric conditions are extracted from the
single scattering data stored in *scat_data*, i.e., one array
element per 'particles' instance in *abs_species* is required. Number
densities are stored in *vmr_field_raw* or *vmr_field* as for all
*abs_species*, but can be taken from (raw) pnd_field type data.

Note that the absorption coefficient is applied both in the
extinction term (neglecting scattering out of the line of sight)
and the emission term (neglecting the scattering source term, i.e.
scattering into the line of sight).

Optionally, particle extinction (sum of absorption and scattering
coefficient) can be used instead of absorption only. To choose this
case, set the *use_abs_as_ext* flag to 0. However, be aware that
this creates some unphysical emission term, hence is only suitable,
where the source term is negligible anyways, e.g. for occultation
simulations.

A line-of-sight direction *rtp_los* is required as particles can
exhibit directional dependent absorption properties, which is taken
into account by this method.
*ScatElementsToabs_speciesAdd* can be used to add all required
settings/data for individual scattering elements at once, i.e. a
 'particles' tag to *abs_species*, a set of single scattering data to
*scat_data* and a number density field to *vmr_field_raw*
(*vmr_field* is derived applying AtmFieldsCalc once VMRs for all
*abs_species* have been added) is appended for each scattering
element.

Like all 'propmat_clearskyAdd*' methods, the method is additive,
i.e., does not overwrite the propagation matrix *propmat_clearsky*,
but adds further contributions.

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] use_abs_as_ext - A flag with value 1 or 0. If set to one, particle
absorption is used in extinction and emission parts of the RT equation, and
scattering out of LOS as well as into LOS is neglected. Otherwise, particle
extinction (absorption+scattering) is applied in both the extinction as well as
the emission part of the RT equation. That is, true extinction is applied, but
emission also includes a pseudo-emission contribution from
the scattering coefficient.
 (default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void propmat_clearskyAddParticles(Workspace& ws,
                                  const Index& use_abs_as_ext = 1) {
  propmat_clearskyAddParticles(
      Var::propmat_clearsky(ws), Var::dpropmat_clearsky_dx(ws),
      Var::stokes_dim(ws), Var::atmosphere_dim(ws), Var::f_grid(ws),
      Var::abs_species(ws), Var::jacobian_quantities(ws), Var::rtp_vmr(ws),
      Var::rtp_los(ws), Var::rtp_temperature(ws), Var::scat_data(ws),
      Var::scat_data_checked(ws), use_abs_as_ext, Var::verbosity(ws));
}

/*! Calculates Zeeman-affected polarized propagation matrix and its
derivatives.

Otherwise as *propmat_clearskyAddFromLookup*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] manual_zeeman_tag - Manual angles tag (default: 0)
@param[in] manual_zeeman_magnetic_field_strength - Manual Magnetic Field
Strength (default: 1.0)
@param[in] manual_zeeman_theta - Manual theta given positive tag (default: 0.0)
@param[in] manual_zeeman_eta - Manual eta given positive tag (default: 0.0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void propmat_clearskyAddZeeman(
    Workspace& ws, const Index& manual_zeeman_tag = 0,
    const Numeric& manual_zeeman_magnetic_field_strength = 1.0,
    const Numeric& manual_zeeman_theta = 0.0,
    const Numeric& manual_zeeman_eta = 0.0) {
  propmat_clearskyAddZeeman(
      Var::propmat_clearsky(ws), Var::nlte_source(ws),
      Var::dpropmat_clearsky_dx(ws), Var::dnlte_dx_source(ws),
      Var::nlte_dsource_dx(ws), Var::abs_lines_per_species(ws), Var::f_grid(ws),
      Var::abs_species(ws), Var::jacobian_quantities(ws),
      Var::isotopologue_ratios(ws), Var::partition_functions(ws),
      Var::rtp_pressure(ws), Var::rtp_temperature(ws), Var::rtp_nlte(ws),
      Var::rtp_vmr(ws), Var::rtp_mag(ws), Var::rtp_los(ws),
      Var::atmosphere_dim(ws), Var::lbl_checked(ws), manual_zeeman_tag,
      manual_zeeman_magnetic_field_strength, manual_zeeman_theta,
      manual_zeeman_eta, Var::verbosity(ws));
}

/*! Sets *propmat_clearsky* to match zero attenuation
if negative value.  Useful for line mixing in some cases.

Use this method just if you know what you are doing!

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void propmat_clearskyForceNegativeToZero(Workspace& ws) {
  propmat_clearskyForceNegativeToZero(Var::propmat_clearsky(ws),
                                      Var::verbosity(ws));
}

/*! Initialize *propmat_clearsky* and *nlte_source*.

This method must be used inside *propmat_clearsky_agenda* and then
be called first.

@author Oliver Lemke, Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void propmat_clearskyInit(Workspace& ws) {
  propmat_clearskyInit(Var::propmat_clearsky(ws), Var::nlte_source(ws),
                       Var::dpropmat_clearsky_dx(ws), Var::dnlte_dx_source(ws),
                       Var::nlte_dsource_dx(ws), Var::abs_species(ws),
                       Var::jacobian_quantities(ws), Var::f_grid(ws),
                       Var::stokes_dim(ws),
                       Var::propmat_clearsky_agenda_checked(ws),
                       Var::nlte_do(ws), Var::verbosity(ws));
}

/*! Sets *propmat_clearsky* to match zero attenuation.

Use this method just if you know what you are doing!

If you want to make a calculation with no clear-sky attenuation at
all, fill *propmat_clearsky_agenda* with this method and required
Ignore statements (don't include *propmat_clearskyInit*).

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void propmat_clearskyZero(Workspace& ws) {
  propmat_clearskyZero(Var::propmat_clearsky(ws), Var::f_grid(ws),
                       Var::stokes_dim(ws), Var::verbosity(ws));
}

/*! Checks if the *propmat_clearsky_agenda* contains all necessary
methods to calculate all the species in *abs_species*.

This method should be called just before the *propmat_clearsky_agenda*
is used, e.g. *DoitGetIncoming*, *ybatchCalc*, *yCalc*

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void propmat_clearsky_agenda_checkedCalc(Workspace& ws) {
  propmat_clearsky_agenda_checkedCalc(
      ws, Var::propmat_clearsky_agenda_checked(ws), Var::abs_species(ws),
      Var::propmat_clearsky_agenda(ws), Var::verbosity(ws));
}

/*! Calculate (vector) gas absorption coefficients for all points in the
atmosphere.

This is useful in two different contexts:

1. For testing and plotting gas absorption. (For RT calculations, gas
absorption is calculated or extracted locally, therefore there is no
need to calculate a global field. But this method is handy for easy
plotting of absorption vs. pressure, for example.)

2. Inside the scattering region, monochromatic absorption is
pre-calculated for the entire atmospheric field.

The calculation itself is performed by the
*propmat_clearsky_agenda*.

@author Stefan Buehler, Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] doppler - A vector of doppler shift values in Hz. Must either be
empty or have same dimension as p_grid. (default: {})
@param[in] los - Line of sight (default: {})

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void propmat_clearsky_fieldCalc(Workspace& ws, const Vector& doppler = {},
                                const Vector& los = {}) {
  propmat_clearsky_fieldCalc(
      ws, Var::propmat_clearsky_field(ws), Var::nlte_source_field(ws),
      Var::atmfields_checked(ws), Var::f_grid(ws), Var::stokes_dim(ws),
      Var::p_grid(ws), Var::lat_grid(ws), Var::lon_grid(ws), Var::t_field(ws),
      Var::vmr_field(ws), Var::nlte_field(ws), Var::mag_u_field(ws),
      Var::mag_v_field(ws), Var::mag_w_field(ws),
      Var::propmat_clearsky_agenda(ws), doppler, los, Var::verbosity(ws));
}

/*! Abel and Boutle [2012] particle size distribution for rain.

Reference: Abel and Boutle, An improved representation of the
raindrop size distribution for single-moment microphysics schemes,
QJRMS, 2012.

This is a 1-parameter PSD, i.e. *pnd_agenda_input* shall have one
column and *pnd_agenda_input_names* shall contain a single string.
The input data in *pnd_agenda_input* shall be rain mass content in
unit of [kg/m3]. The naming used is *pnd_agenda_input_names* is free
but the same name must be used in *particle_bulkprop_names* and
*dpnd_data_dx_names*.

Particles are assumed to be near-spherical, ie. *psd_size_grid* can
either be in terms of volume (or mass) equivalent diameter or
maximum diameter.

Derivatives are obtained analytically.

The validity range of mass content is not limited. Negative mass
contents will produce negative psd values following a distribution
given by abs(RWC), ie. abs(psd)=f(abs(RWC)).

If temperature is outside [*t_min*,*t_max*] psd=0 and dpsd=0 if
picky=0, or an error is thrown if picky=1.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] t_min - Low temperature limit to calculate a psd. (default: 273)
@param[in] t_max - High temperature limit to calculate a psd. (default: 373)
@param[in] picky - Flag whether to be strict with parametrization value checks.
(default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void psdAbelBoutle12(Workspace& ws, const Numeric& t_min = 273,
                     const Numeric& t_max = 373, const Index& picky = 0) {
  psdAbelBoutle12(Var::psd_data(ws), Var::dpsd_data_dx(ws),
                  Var::psd_size_grid(ws), Var::pnd_agenda_input_t(ws),
                  Var::pnd_agenda_input(ws), Var::pnd_agenda_input_names(ws),
                  Var::dpnd_data_dx_names(ws), Var::scat_species_a(ws),
                  Var::scat_species_b(ws), t_min, t_max, picky,
                  Var::verbosity(ws));
}

/*! Normalized PSD as proposed in Delanoë et al. ((2014)),

Title and journal:
'Normalized particle size distribution for remote sensing
application', J. Geophys. Res. Atmos., 119, 4204–422.

The PSD has two independent parameters *n0Star*, the intercept
parameter, and *Dm*, the volume-weighted diameter.
This implementation expects as input two out of the following
three quantities: *iwc*, *n0Star*, *Dm*. In this case one of
the input parameters *iwc*, *n0Star*, *Dm* must be set to -999.
*It is also possible to provide only *iwc*, in which case an a
priori assumption will be used to deduce *n0Star* from temperature.
In this case both *n0Star* and *Dm* must be set to -999.0.

This PSD is not defined for vanishing concentrations of
scatterers as it requires normalization by *Dm*. It is up
to the user to ensure that the value of *Dm* is sufficiently
large. An error is thrown if *Dm* is zero or below the value
provided by *dm_min*.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[in] iwc - Ice water content (default:
std::numeric_limits<Numeric>::quiet_NaN())
@param[in] n0Star - Intercept parameter (default:
std::numeric_limits<Numeric>::quiet_NaN())
@param[in] Dm - Volume weighted diameter (default:
std::numeric_limits<Numeric>::quiet_NaN())
@param[in] rho - Density of ice (default: 917.6)
@param[in] alpha - *alpha* parameter of the shape function (default: -0.237)
@param[in] beta - *beta* paramter of the shape function (default: 1.839)
@param[in] t_min - Low temperature limit to calculate a psd.
@param[in] t_max - High temperature limit to calculate a psd.
@param[in] dm_min - Lower threshold for *Dm* below which an error is thrown.
(default: -1.0)
@param[in] picky - Flag whether to be strict with parametrization value checks.
(default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void psdDelanoeEtAl14(
    Workspace& ws, const Numeric& t_min, const Numeric& t_max,
    const Numeric& iwc = std::numeric_limits<Numeric>::quiet_NaN(),
    const Numeric& n0Star = std::numeric_limits<Numeric>::quiet_NaN(),
    const Numeric& Dm = std::numeric_limits<Numeric>::quiet_NaN(),
    const Numeric& rho = 917.6, const Numeric& alpha = -0.237,
    const Numeric& beta = 1.839, const Numeric& dm_min = -1.0,
    const Index& picky = 0) {
  psdDelanoeEtAl14(Var::psd_data(ws), Var::dpsd_data_dx(ws),
                   Var::psd_size_grid(ws), Var::pnd_agenda_input_t(ws),
                   Var::pnd_agenda_input(ws), Var::pnd_agenda_input_names(ws),
                   Var::dpnd_data_dx_names(ws), iwc, n0Star, Dm, rho, alpha,
                   beta, t_min, t_max, dm_min, picky, Var::verbosity(ws));
}

/*! The Field et al. [2007] particle size distribution for snow and
cloud ice.

This is a 1-parameter PSD, i.e. *pnd_agenda_input* shall have one
column and *pnd_agenda_input_names* shall contain a single string.
The input data in *pnd_agenda_input* shall be ice hydrometeor mass
content in unit of [kg/m3]. The naming used is *pnd_agenda_input_names*
is free but the same name must be used in *particle_bulkprop_names* and
*dpnd_data_dx_names*.

*psd_size_grid* shall contain size in terms of maximum diameter.

Derivatives are obtained by perturbation of 0.1%, but not less than
1e-9 kg/m3.

Both parametrization for tropics and midlatitudes are handled,
governed by setting of *regime*, where "TR" selectes the tropical
case, and "ML" the midlatitude one.

The validity range of mass content is not limited. Negative mass
contents will produce negative psd values following a distribution
given by abs(IWC), ie. abs(psd)=f(abs(IWC)).

If temperature is outside [*t_min*,*t_max*] psd=0 and dpsd=0 if
picky=0, or an error is thrown if picky=1.

For temperatures below *t_min_psd*, the size distribution is
calculated for T = *t_min_psd*. Likewise, for temperatures above
*t_max_psd*, the distribution is derived for T = *t_max_psd*.

Defaults of *t_min_psd* and *t_max_psd* were set considering that
the parametrization has been derived from measurements over
temperatures of -60C to 0C.
Checks of the sanity of the mass-dimension relationship are performed
Errors are thrown if:
- Mass-dimension relation exponent *scat_species_b* is outside
  [*beta_min*, *beta_max*].

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] regime - Parametrization regime ("TR"=tropical or "ML"=midlatitude).
@param[in] t_min - Low temperature limit to calculate a psd. (default: 0)
@param[in] t_max - High temperature limit to calculate a psd. (default: 290.)
@param[in] t_min_psd - Low temperature limit to use as paramtrization
temperature. (default: 200.)
@param[in] t_max_psd - High temperature limit to use as paramtrization
temperature. (default: 273.15)
@param[in] beta_min - Low *b* limit (only if picky). (default: 1.01)
@param[in] beta_max - High *b* limit (only if picky). (default: 4)
@param[in] picky - Flag whether to be strict with parametrization value checks.
(default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void psdFieldEtAl07(Workspace& ws, const String& regime,
                    const Numeric& t_min = 0, const Numeric& t_max = 290.,
                    const Numeric& t_min_psd = 200.,
                    const Numeric& t_max_psd = 273.15,
                    const Numeric& beta_min = 1.01, const Numeric& beta_max = 4,
                    const Index& picky = 0) {
  psdFieldEtAl07(Var::psd_data(ws), Var::dpsd_data_dx(ws),
                 Var::psd_size_grid(ws), Var::pnd_agenda_input_t(ws),
                 Var::pnd_agenda_input(ws), Var::pnd_agenda_input_names(ws),
                 Var::dpnd_data_dx_names(ws), Var::scat_species_a(ws),
                 Var::scat_species_b(ws), regime, t_min, t_max, t_min_psd,
                 t_max_psd, beta_min, beta_max, picky, Var::verbosity(ws));
}

/*! The Field [2019] particle size distribution for hail.

Reference: Field, Normalized hail particle size distributions from the T-28
storm-penetrating aircraft, JAMC, 2019

This is a 1-parmater PSD i.e. *pnd_agenda_input* shall have one column and
*pnd_agenda_input_names* shall contain a single string.
The input data in *pnd_agenda_input* shall be hail mass content in
unit of [kg/m3]. The naming used is *pnd_agenda_input_names* is free
but the same name must be used in *particle_bulkprop_names* and
*dpnd_data_dx_names*.
The parameters assume a constant effective density, i.e. scat_species_b pprox 3

Derivatives are obtained analytically.

The validity range of mass content is not limited. Negative mass
contents will produce negative psd values following a distribution
given by abs(HWC), ie. abs(psd)=f(abs(HWC)).

If temperature is outside [*t_min*,*t_max*] psd=0 and dpsd=0 if
picky=0, or an error is thrown if picky=1.

@author Stuart Fox

@param[in,out] Workspace ws - An ARTS workspace
@param[in] t_min - Low temperature limit to calculate a psd.
@param[in] t_max - High temperature limit to calculate a psd.
@param[in] picky - Flag whether to be strict with parametrization value checks.
(default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void psdFieldEtAl19(Workspace& ws, const Numeric& t_min, const Numeric& t_max,
                    const Index& picky = 0) {
  psdFieldEtAl19(Var::psd_data(ws), Var::dpsd_data_dx(ws),
                 Var::psd_size_grid(ws), Var::pnd_agenda_input_t(ws),
                 Var::pnd_agenda_input(ws), Var::pnd_agenda_input_names(ws),
                 Var::dpnd_data_dx_names(ws), Var::scat_species_a(ws),
                 Var::scat_species_b(ws), t_min, t_max, picky,
                 Var::verbosity(ws));
}

/*! McFarquahar and Heymsfield [1997] particle size distribution
for cloud ice.

This is a 1-parameter PSD, i.e. *pnd_agenda_input* shall have one
column and *pnd_agenda_input_names* shall contain a single string.
The input data in *pnd_agenda_input* shall be ice hydrometeor mass
content in unit of [kg/m3]. The naming used is *pnd_agenda_input_names*
is free but the same name must be used in *particle_bulkprop_names* and
*dpnd_data_dx_names*.

*psd_size_grid* shall contain size in terms of volume equivalent diameter.

Derivatives are obtained by perturbation of 0.1%, but not less than
1e-9 kg/m3.

The validity range of mass content is not limited. Negative mass
contents will produce negative psd values following a distribution
given by abs(IWC), ie. abs(psd)=f(abs(IWC)).

If temperature is outside [*t_min*,*t_max*] psd=0 and dpsd=0 if
picky=0, or an error is thrown if picky=1.

For temperatures below *t_min_psd*, the size distribution is
calculated for T = *t_min_psd*. Likewise, for temperatures above
*t_max_psd*, the distribution is derived for T = *t_max_psd*.

Defaults of *t_min_psd* and *t_max_psd* were set considering that
the parametrization has been derived from measurements over
temperatures of -70C to -20C.
The noisy option can not be used together with calculation of
derivatives (ie. when *dpnd_data_dx_names* is not empty).

@author Patrick Eriksson, Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] t_min - Low temperature limit to calculate a psd. (default: 0)
@param[in] t_max - High temperature limit to calculate a psd. (default: 280.)
@param[in] t_min_psd - Low temperature limit to use as paramtrization
temperature. (default: 180)
@param[in] t_max_psd - High temperature limit to use as paramtrization
temperature. (default: 273.15)
@param[in] picky - Flag whether to be strict with parametrization value checks.
(default: 0)
@param[in] noisy - Distribution parameter perturbance flag (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void psdMcFarquaharHeymsfield97(Workspace& ws, const Numeric& t_min = 0,
                                const Numeric& t_max = 280.,
                                const Numeric& t_min_psd = 180,
                                const Numeric& t_max_psd = 273.15,
                                const Index& picky = 0,
                                const Index& noisy = 0) {
  psdMcFarquaharHeymsfield97(
      Var::psd_data(ws), Var::dpsd_data_dx(ws), Var::psd_size_grid(ws),
      Var::pnd_agenda_input_t(ws), Var::pnd_agenda_input(ws),
      Var::pnd_agenda_input_names(ws), Var::dpnd_data_dx_names(ws),
      Var::scat_species_a(ws), Var::scat_species_b(ws), t_min, t_max, t_min_psd,
      t_max_psd, picky, noisy, Var::verbosity(ws));
}

/*! Calculates *psd_data* and  *dpsd_data_dx* following Milbrandt and Yau (2005)
two moment particle size distribution for cloud water, cloud ice,
rain, snow, graupel and hail, which is used in the GEM model.

WSM for use in *pnd_agenda_array* for mapping *particle_bulkprop_field*
to *pnd_field* using *pnd_fieldCalcFromParticleBulkProps*.
Produces the particle size distribution values (dN/dD) and their
derivates with respect to independent variables x by *dpnd_data_dx_names*
over multiple particle sizes and atmospheric levels (or SWC/T
combinations).

*psd_size_grid* is considered to be in terms of maximum diameter.
WC is considered to be in terms of mass content (or mass density),
ie. units of [kg/m3]. N_tot in terms of number density, ie. units of [1/m3] .
Derivatives with respect to WC and N_tot are obtained analytically.

Six particle size distributions for the different hydrometeors are handled,
governed by setting of *hydrometeor_type*, where
    "cloud_water" selects cloud liquid water ,
    "cloud_ice" selects cloud ice,
    "snow" selects snow,
    "rain" selects rain,
    "graupel" selects graupel, and
    "hail" selects hail,

Requirements:

*pnd_agenda_input_names* must include :
    ["X-mass_density", "X-number_density" ]. "X" is an arbitrary name
The entries in  *dpnd_data_dx_names* (ie. the allowed
independent variablea ) can be "X-mass_density" and\or
"X-number_density".

The validity range of WC is not limited. Negative WC will produce
negative psd values following a distribution given by abs(WC), ie.
abs(psd)=f(abs(WC)).

If temperature is outside [*t_min*,*t_max*] psd=0 and dpsd=0 if
picky=0, or an error is thrown if picky=1.

@author Manfred Brath

@param[in,out] Workspace ws - An ARTS workspace
@param[in] hydrometeor_type - Hydrometeor type (see above description).
@param[in] t_min - Low temperature limit to calculate a psd. (default: 0)
@param[in] t_max - High temperature limit to calculate a psd. (default: 999)
@param[in] picky - Flag whether to be strict with parametrization value checks.
(default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void psdMilbrandtYau05(Workspace& ws, const String& hydrometeor_type,
                       const Numeric& t_min = 0, const Numeric& t_max = 999,
                       const Index& picky = 0) {
  psdMilbrandtYau05(Var::psd_data(ws), Var::dpsd_data_dx(ws),
                    Var::psd_size_grid(ws), Var::pnd_agenda_input_t(ws),
                    Var::pnd_agenda_input(ws), Var::pnd_agenda_input_names(ws),
                    Var::dpnd_data_dx_names(ws), hydrometeor_type, t_min, t_max,
                    picky, Var::verbosity(ws));
}

/*! Modified gamma distribution PSD using n0, mu, la and ga as parameters.

The modified gamma distribution is a 4-parameter (n0, mu, la and ga)
distribution [Petty & Huang, JAS, 2011)]:
   n(x) = n0 * x^mu * exp( -la*x^ga )
where x is particle size or mass.

The parameters can be given in two ways, either by *pnd_agenda_input* or
as GIN arguments. The first option allows the parameter to vary, while
in the second case the parameter gets a constant value. If a parameter is
part of *pnd_agenda_input*, the corresponding GIN argument must be set
to NaN (which is default). This means that the number of columns in
*pnd_agenda_input* and the number of non-NaN choices for n0, mu, la and
ga must add up to four.

Data in *pnd_agenda_input* are linked to the MGD parameters in term of
order, the naming in *pnd_agenda_input_names* is free. If all four
parameteras are specified by *pnd_agenda_input*, the data in the first
column are taken as n0, the second column as mu etc. If a parameter is
given as a GIN argument, the columns are just shifted with one position.
For example, if mu and ga are specified as GIN arguments, *pnd_agenda_input*
shall have two columns, with n0-values in the first one and la-values in
the second one.

The GIN route is especially suitable for selecting special cases of MGD.
For example, by setting mu=0 and ga=1, an exponential PSD is obtained:
   n(x) = n0 * exp( -la*x )
With mu=1 and ga=1, the gamma PSD is obtained:
   n(x) = n0 * x^mu *exp( -la*x )
There should be little overhead in using the method for exponential
and gamma PSDs, there is an internal switch to dedicated expressions
for those PSDs.

Derivatives can only be obtained for parameters that are specified by
*pnd_agenda_input*.

If temperature is outside [*t_min*,*t_max*] psd=0 and dpsd=0 if
picky=0, or an error is thrown if picky=1.

These requirements apply to the MGD parameters:
  la > 0
  ga > 0

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] n0 - n0 (default: std::numeric_limits<Numeric>::quiet_NaN())
@param[in] mu - mu (default: std::numeric_limits<Numeric>::quiet_NaN())
@param[in] la - la (default: std::numeric_limits<Numeric>::quiet_NaN())
@param[in] ga - ga (default: std::numeric_limits<Numeric>::quiet_NaN())
@param[in] t_min - Low temperature limit to calculate a psd.
@param[in] t_max - High temperature limit to calculate a psd.
@param[in] picky - Flag whether to be strict with parametrization value checks.
(default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void psdModifiedGamma(
    Workspace& ws, const Numeric& t_min, const Numeric& t_max,
    const Numeric& n0 = std::numeric_limits<Numeric>::quiet_NaN(),
    const Numeric& mu = std::numeric_limits<Numeric>::quiet_NaN(),
    const Numeric& la = std::numeric_limits<Numeric>::quiet_NaN(),
    const Numeric& ga = std::numeric_limits<Numeric>::quiet_NaN(),
    const Index& picky = 0) {
  psdModifiedGamma(Var::psd_data(ws), Var::dpsd_data_dx(ws),
                   Var::psd_size_grid(ws), Var::pnd_agenda_input_t(ws),
                   Var::pnd_agenda_input(ws), Var::pnd_agenda_input_names(ws),
                   Var::dpnd_data_dx_names(ws), n0, mu, la, ga, t_min, t_max,
                   picky, Var::verbosity(ws));
}

/*! Modified gamma distribution (MGD) PSD, with mass content as input.

See *psdModifiedGamma* for a defintion of MGD parameters and how
this PSD is handled in ARTS. Only deviations with respect to
*psdModifiedGamma* are described here.

This version of MGD PSD takes mass content as first input argument.
This means that the first column of *pnd_agenda_input* shall hold
mass content data.

The mass content basically replaces one of the standard parameters
(n0, mu, la and ga). This parameter is denoted as the dependent one.
The dependent parameter is selected by setting the corresponding GIN
to -999. So far only n0 and la are allowed to be dependent.

Regarding remaining columns in *pnd_agenda_input* and constant
parameter values (by GIN) follows the same principle as for
*psdModifiedGamma* except that mass is always in column one (as
mentioned) and that there is no position in *pnd_agenda_input*
for the dependent parameter.

These requirements apply to the MGD parameters:
  mu + scat_species_b + 1 > 0
  la > 0
  ga > 0
  If la is the dependent parameter, mass content must be > 0.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] n0 - n0 (default: std::numeric_limits<Numeric>::quiet_NaN())
@param[in] mu - mu (default: std::numeric_limits<Numeric>::quiet_NaN())
@param[in] la - la (default: std::numeric_limits<Numeric>::quiet_NaN())
@param[in] ga - ga (default: std::numeric_limits<Numeric>::quiet_NaN())
@param[in] t_min - Low temperature limit to calculate a psd.
@param[in] t_max - High temperature limit to calculate a psd.
@param[in] picky - Flag whether to be strict with parametrization value checks.
(default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void psdModifiedGammaMass(
    Workspace& ws, const Numeric& t_min, const Numeric& t_max,
    const Numeric& n0 = std::numeric_limits<Numeric>::quiet_NaN(),
    const Numeric& mu = std::numeric_limits<Numeric>::quiet_NaN(),
    const Numeric& la = std::numeric_limits<Numeric>::quiet_NaN(),
    const Numeric& ga = std::numeric_limits<Numeric>::quiet_NaN(),
    const Index& picky = 0) {
  psdModifiedGammaMass(
      Var::psd_data(ws), Var::dpsd_data_dx(ws), Var::psd_size_grid(ws),
      Var::pnd_agenda_input_t(ws), Var::pnd_agenda_input(ws),
      Var::pnd_agenda_input_names(ws), Var::dpnd_data_dx_names(ws),
      Var::scat_species_a(ws), Var::scat_species_b(ws), n0, mu, la, ga, t_min,
      t_max, picky, Var::verbosity(ws));
}

/*! Modified gamma distribution PSD, with mass content and mean particle
mass (Mmean) as inputs.

"Mean particle mass" is here defined as the mass content divided with
the total number density.

This version of MGD PSD works as *psdModifiedGammaMass*, but takes
mass content and mean particle mass as first two arguments. This
means that the first and second column of *pnd_agenda_input* shall
hold mass content and Mmean, respectively. Accordingly, the number
of dependent parameters is two.

These requirements apply to the MGD parameters:
  mu + 1 > 0
  la > 0
  ga > 0
  Mmean must be > 0.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] n0 - n0 (default: std::numeric_limits<Numeric>::quiet_NaN())
@param[in] mu - mu (default: std::numeric_limits<Numeric>::quiet_NaN())
@param[in] la - la (default: std::numeric_limits<Numeric>::quiet_NaN())
@param[in] ga - ga (default: std::numeric_limits<Numeric>::quiet_NaN())
@param[in] t_min - Low temperature limit to calculate a psd.
@param[in] t_max - High temperature limit to calculate a psd.
@param[in] picky - Flag whether to be strict with parametrization value checks.
(default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void psdModifiedGammaMassMeanParticleMass(
    Workspace& ws, const Numeric& t_min, const Numeric& t_max,
    const Numeric& n0 = std::numeric_limits<Numeric>::quiet_NaN(),
    const Numeric& mu = std::numeric_limits<Numeric>::quiet_NaN(),
    const Numeric& la = std::numeric_limits<Numeric>::quiet_NaN(),
    const Numeric& ga = std::numeric_limits<Numeric>::quiet_NaN(),
    const Index& picky = 0) {
  psdModifiedGammaMassMeanParticleMass(
      Var::psd_data(ws), Var::dpsd_data_dx(ws), Var::psd_size_grid(ws),
      Var::pnd_agenda_input_t(ws), Var::pnd_agenda_input(ws),
      Var::pnd_agenda_input_names(ws), Var::dpnd_data_dx_names(ws),
      Var::scat_species_a(ws), Var::scat_species_b(ws), n0, mu, la, ga, t_min,
      t_max, picky, Var::verbosity(ws));
}

/*! Modified gamma distribution PSD, with mass content and total number
density (Ntot) as inputs.

This version of MGD PSD works as *psdModifiedGammaMass*, but takes
mass content and total number density as first two arguments. This
means that the first and second column of *pnd_agenda_input* shall
hold mass content and Ntot, respectively. Accordingly, the number
of dependent parameters is two.

These requirements apply:
  mu + 1 > 0
  la > 0
  ga > 0
  Ntot must be > 0.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] n0 - n0 (default: std::numeric_limits<Numeric>::quiet_NaN())
@param[in] mu - mu (default: std::numeric_limits<Numeric>::quiet_NaN())
@param[in] la - la (default: std::numeric_limits<Numeric>::quiet_NaN())
@param[in] ga - ga (default: std::numeric_limits<Numeric>::quiet_NaN())
@param[in] t_min - Low temperature limit to calculate a psd.
@param[in] t_max - High temperature limit to calculate a psd.
@param[in] picky - Flag whether to be strict with parametrization value checks.
(default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void psdModifiedGammaMassNtot(
    Workspace& ws, const Numeric& t_min, const Numeric& t_max,
    const Numeric& n0 = std::numeric_limits<Numeric>::quiet_NaN(),
    const Numeric& mu = std::numeric_limits<Numeric>::quiet_NaN(),
    const Numeric& la = std::numeric_limits<Numeric>::quiet_NaN(),
    const Numeric& ga = std::numeric_limits<Numeric>::quiet_NaN(),
    const Index& picky = 0) {
  psdModifiedGammaMassNtot(
      Var::psd_data(ws), Var::dpsd_data_dx(ws), Var::psd_size_grid(ws),
      Var::pnd_agenda_input_t(ws), Var::pnd_agenda_input(ws),
      Var::pnd_agenda_input_names(ws), Var::dpnd_data_dx_names(ws),
      Var::scat_species_a(ws), Var::scat_species_b(ws), n0, mu, la, ga, t_min,
      t_max, picky, Var::verbosity(ws));
}

/*! Modified gamma distribution PSD, with mass content as input.

The intercept parameter N0 is assumed dependent on the slope parameter
lambda, such that N0=N_alpha*lambda^n_b with fixed N_alpha and n_b.
This is a common form for many PSD parametrizations for use with
single-moment mass-based schemes.

This version of MGD PSD takes mass content as first input argument.
This means that the first column of *pnd_agenda_input* shall hold
mass content data. The dependent parameter is assumed to be lambda.

@author Stuart Fox

@param[in,out] Workspace ws - An ARTS workspace
@param[in] n_alpha - n_alpha
@param[in] n_b - n_b
@param[in] mu - mu
@param[in] gamma - gamma
@param[in] t_min - Low temperature limit to calculate a psd.
@param[in] t_max - High temperature limit to calculate a psd.
@param[in] picky - Flag whether to be strict with parametrization value checks.
(default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void psdModifiedGammaMassSingleMoment(Workspace& ws, const Numeric& n_alpha,
                                      const Numeric& n_b, const Numeric& mu,
                                      const Numeric& gamma,
                                      const Numeric& t_min,
                                      const Numeric& t_max,
                                      const Index& picky = 0) {
  psdModifiedGammaMassSingleMoment(
      Var::psd_data(ws), Var::dpsd_data_dx(ws), Var::psd_size_grid(ws),
      Var::pnd_agenda_input_t(ws), Var::pnd_agenda_input(ws),
      Var::pnd_agenda_input_names(ws), Var::dpnd_data_dx_names(ws),
      Var::scat_species_a(ws), Var::scat_species_b(ws), n_alpha, n_b, mu, gamma,
      t_min, t_max, picky, Var::verbosity(ws));
}

/*! Modified gamma distribution PSD, with mass content and mean size
(Xmean) as inputs.

"Mean size" is here defined as mass weighted size. Remembering that
mass is a*x^b, this mean size can be expressed as M_b+1/M_b where M_b
is b:th moment of the PSD (see e.g. Eq. 17 in Petty&Huang, JAS, 2011).

This version of MGD PSD works as *psdModifiedGammaMass*, but takes
mass content and mass size as first two arguments. This means that
the first and second column of *pnd_agenda_input* shall hold mass
content and Xmean, respectively. Accordingly, the number of dependent
parameters is two.

These requirements apply to the MGD parameters:
  mu + scat_species_b + 1 > 0
  la > 0
  ga > 0
  Xmean must be > 0.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] n0 - n0 (default: std::numeric_limits<Numeric>::quiet_NaN())
@param[in] mu - mu (default: std::numeric_limits<Numeric>::quiet_NaN())
@param[in] la - la (default: std::numeric_limits<Numeric>::quiet_NaN())
@param[in] ga - ga (default: std::numeric_limits<Numeric>::quiet_NaN())
@param[in] t_min - Low temperature limit to calculate a psd.
@param[in] t_max - High temperature limit to calculate a psd.
@param[in] picky - Flag whether to be strict with parametrization value checks.
(default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void psdModifiedGammaMassXmean(
    Workspace& ws, const Numeric& t_min, const Numeric& t_max,
    const Numeric& n0 = std::numeric_limits<Numeric>::quiet_NaN(),
    const Numeric& mu = std::numeric_limits<Numeric>::quiet_NaN(),
    const Numeric& la = std::numeric_limits<Numeric>::quiet_NaN(),
    const Numeric& ga = std::numeric_limits<Numeric>::quiet_NaN(),
    const Index& picky = 0) {
  psdModifiedGammaMassXmean(
      Var::psd_data(ws), Var::dpsd_data_dx(ws), Var::psd_size_grid(ws),
      Var::pnd_agenda_input_t(ws), Var::pnd_agenda_input(ws),
      Var::pnd_agenda_input_names(ws), Var::dpnd_data_dx_names(ws),
      Var::scat_species_a(ws), Var::scat_species_b(ws), n0, mu, la, ga, t_min,
      t_max, picky, Var::verbosity(ws));
}

/*! Modified gamma distribution PSD, with mass content and median size
(Xmedian) as inputs.


This version of MGD PSD works as *psdModifiedGammaMass*, but takes
mass content and median size as first two arguments. This means that
the first and second column of *pnd_agenda_input* shall hold mass
content and Xmedian, respectively. Accordingly, the number of
dependent parameters is two.

These requirements apply to the MGD parameters:
  mu + scat_species_b + 1 > 0
  la > 0
  ga > 0
  Xmedian must be > 0.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] n0 - n0 (default: std::numeric_limits<Numeric>::quiet_NaN())
@param[in] mu - mu (default: std::numeric_limits<Numeric>::quiet_NaN())
@param[in] la - la (default: std::numeric_limits<Numeric>::quiet_NaN())
@param[in] ga - ga (default: std::numeric_limits<Numeric>::quiet_NaN())
@param[in] t_min - Low temperature limit to calculate a psd.
@param[in] t_max - High temperature limit to calculate a psd.
@param[in] picky - Flag whether to be strict with parametrization value checks.
(default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void psdModifiedGammaMassXmedian(
    Workspace& ws, const Numeric& t_min, const Numeric& t_max,
    const Numeric& n0 = std::numeric_limits<Numeric>::quiet_NaN(),
    const Numeric& mu = std::numeric_limits<Numeric>::quiet_NaN(),
    const Numeric& la = std::numeric_limits<Numeric>::quiet_NaN(),
    const Numeric& ga = std::numeric_limits<Numeric>::quiet_NaN(),
    const Index& picky = 0) {
  psdModifiedGammaMassXmedian(
      Var::psd_data(ws), Var::dpsd_data_dx(ws), Var::psd_size_grid(ws),
      Var::pnd_agenda_input_t(ws), Var::pnd_agenda_input(ws),
      Var::pnd_agenda_input_names(ws), Var::dpnd_data_dx_names(ws),
      Var::scat_species_a(ws), Var::scat_species_b(ws), n0, mu, la, ga, t_min,
      t_max, picky, Var::verbosity(ws));
}

/*! Mono-dispersive PSD, with number density given.

This is a 1-parameter PSD, i.e. *pnd_agenda_input* shall have one
column and *pnd_agenda_input_names* shall contain a single string.
The input data in *pnd_agenda_input* shall be number densities, in
unit of [#/m3]. The naming used is *pnd_agenda_input_names* is free
but the same name must be used in *particle_bulkprop_names* and
*dpnd_data_dx_names*.

The method checks that the scattering species indicated (by
*species_index*) has a single element, and just inserts the provided
number density in *psd_data*.

If temperature is outside [*t_min*,*t_max*] psd=0 and dpsd=0 if
picky=0, or an error is thrown if picky=1.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] species_index - The index of the scattering species of concern
(0-based).
@param[in] t_min - Low temperature limit to calculate a psd.
@param[in] t_max - High temperature limit to calculate a psd.
@param[in] picky - Flag whether to be strict with parametrization value checks.
(default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void psdMonoDispersive(Workspace& ws, const Index& species_index,
                       const Numeric& t_min, const Numeric& t_max,
                       const Index& picky = 0) {
  psdMonoDispersive(Var::psd_data(ws), Var::dpsd_data_dx(ws),
                    Var::pnd_agenda_input_t(ws), Var::pnd_agenda_input(ws),
                    Var::pnd_agenda_input_names(ws),
                    Var::dpnd_data_dx_names(ws), Var::scat_meta(ws),
                    species_index, t_min, t_max, picky, Var::verbosity(ws));
}

/*! Mono-dispersive PSD, with mass content given.

This is a 1-parameter PSD, i.e. *pnd_agenda_input* shall have one
column and *pnd_agenda_input_names* shall contain a single string.
The input data in *pnd_agenda_input* shall be mass contents, in
unit of [#/m3]. The naming used is *pnd_agenda_input_names* is free
but the same name must be used in *particle_bulkprop_names* and
*dpnd_data_dx_names*.

The method checks that the scattering species indicated (by
*species_index*) has a single element, and sets *psd_data* based
on the mass contents given and the particle mass (derived from
*scat_meta*).

If temperature is outside [*t_min*,*t_max*] psd=0 and dpsd=0 if
picky=0, or an error is thrown if picky=1.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] species_index - The index of the scattering species of concern
(0-based).
@param[in] t_min - Low temperature limit to calculate a psd.
@param[in] t_max - High temperature limit to calculate a psd.
@param[in] picky - Flag whether to be strict with parametrization value checks.
(default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void psdMonoMass(Workspace& ws, const Index& species_index,
                 const Numeric& t_min, const Numeric& t_max,
                 const Index& picky = 0) {
  psdMonoMass(Var::psd_data(ws), Var::dpsd_data_dx(ws),
              Var::pnd_agenda_input_t(ws), Var::pnd_agenda_input(ws),
              Var::pnd_agenda_input_names(ws), Var::dpnd_data_dx_names(ws),
              Var::scat_meta(ws), species_index, t_min, t_max, picky,
              Var::verbosity(ws));
}

/*! Calculates *psd_data* and *dpsd_data_dx* following Seifert and Beheng (2006)
two moment particle size distribution for cloud water, cloud ice,
rain, snow, graupel and hail, which is used in the ICON model.

WSM for use in *pnd_agenda_array* for mapping *particle_bulkprop_field*
to *pnd_field* using *pnd_fieldCalcFromParticleBulkProps*.
Produces the particle size distribution values (dN/dD) and their
derivates with respect to independent variables x by *dpnd_data_dx_names*
over multiple particle sizes and atmospheric levels (or SWC/T
combinations).

*psd_size_grid* is considered to be in terms of mass.
WC is considered to be in terms of mass content (or mass density),
ie. units of [kg/m3]. N_tot in terms of number density, ie. units of [1/m3] .
Derivatives with respect to WC and N_tot are obtained analytically.

Six particle size distributions for the different hydrometeors are handled,
governed by setting of *hydrometeor_type*, where
    "cloud_water" selects cloud liquid water ,
    "cloud_ice" selects cloud ice,
    "snow" selects snow,
    "rain" selects rain,
    "graupel" selects graupel, and
    "hail" selects hail,

Requirements:

*pnd_agenda_input_names* must include :
    ["X-mass_density", "X-number_density" ]. "X" is an arbitrary name
The entries in  *dpnd_data_dx_names* (ie. the allowed
independent variablea ) can be "X-mass_density" and\or
"X-number_density".

The validity range of WC is not limited. Negative WC will produce
negative psd values following a distribution given by abs(WC), ie.
abs(psd)=f(abs(WC)).

If temperature is outside [*t_min*,*t_max*] psd=0 and dpsd=0 if
picky=0, or an error is thrown if picky=1.

@author Manfred Brath

@param[in,out] Workspace ws - An ARTS workspace
@param[in] hydrometeor_type - Hydrometeor type (see above description).
@param[in] t_min - Low temperature limit to calculate a psd. (default: 0)
@param[in] t_max - High temperature limit to calculate a psd. (default: 999)
@param[in] picky - Flag whether to be strict with parametrization value checks.
(default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void psdSeifertBeheng06(Workspace& ws, const String& hydrometeor_type,
                        const Numeric& t_min = 0, const Numeric& t_max = 999,
                        const Index& picky = 0) {
  psdSeifertBeheng06(Var::psd_data(ws), Var::dpsd_data_dx(ws),
                     Var::psd_size_grid(ws), Var::pnd_agenda_input_t(ws),
                     Var::pnd_agenda_input(ws), Var::pnd_agenda_input_names(ws),
                     Var::dpnd_data_dx_names(ws), hydrometeor_type, t_min,
                     t_max, picky, Var::verbosity(ws));
}

/*! Wang et al. [2016] particle size distribution for rain.

Reference: Wang et al., Investigation of liquid cloud microphysical
properties of deep convective systems: 1. Parameterization raindrop
size distribution and its application ..., 2016.

This is a 1-parameter PSD, i.e. *pnd_agenda_input* shall have one
column and *pnd_agenda_input_names* shall contain a single string.
The input data in *pnd_agenda_input* shall be rain mass content in
unit of [kg/m3]. The naming used is *pnd_agenda_input_names* is free
but the same name must be used in *particle_bulkprop_names* and
*dpnd_data_dx_names*.

Particles are assumed to be near-spherical, ie. *psd_size_grid* can
either be in terms of volume (or mass) equivalent diameter or
maximum diameter.

Derivatives are obtained analytically.

The validity range of mass content is not limited. Negative mass
contents will produce negative psd values following a distribution
given by abs(RWC), ie. abs(psd)=f(abs(RWC)).

If temperature is outside [*t_min*,*t_max*] psd=0 and dpsd=0 if
picky=0, or an error is thrown if picky=1.

@author Jana Mendrok, Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] t_min - Low temperature limit to calculate a psd. (default: 273)
@param[in] t_max - High temperature limit to calculate a psd. (default: 373)
@param[in] picky - Flag whether to be strict with parametrization value checks.
(default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void psdWangEtAl16(Workspace& ws, const Numeric& t_min = 273,
                   const Numeric& t_max = 373, const Index& picky = 0) {
  psdWangEtAl16(Var::psd_data(ws), Var::dpsd_data_dx(ws),
                Var::psd_size_grid(ws), Var::pnd_agenda_input_t(ws),
                Var::pnd_agenda_input(ws), Var::pnd_agenda_input_names(ws),
                Var::dpnd_data_dx_names(ws), Var::scat_species_a(ws),
                Var::scat_species_b(ws), t_min, t_max, picky,
                Var::verbosity(ws));
}

/*! Earth reference ellipsoids.

The reference ellipsoid (*refellipsoid*) is set to model the Earth,
following different models. The options are:

   "Sphere" : A spherical Earth. The radius is set following
      the value set for the Earth radius in constants.cc.

   "WGS84" : The reference ellipsoid used by the GPS system.
      Should be the standard choice for a non-spherical Earth.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] model - Model ellipsoid to use. Options listed above. (default:
"Sphere")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void refellipsoidEarth(Workspace& ws, const String& model = "Sphere") {
  refellipsoidEarth(Var::refellipsoid(ws), model, Var::verbosity(ws));
}

/*! Io reference ellipsoids.

The reference ellipsoid (*refellipsoid*) is set to model Io,
folowing different models. The options are:

   "Sphere" : A spherical planetesimal. The radius is taken from
      report of the IAU/IAG Working Group.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] model - Model ellipsoid to use. Options listed above. (default:
"Sphere")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void refellipsoidEuropa(Workspace& ws, const String& model = "Sphere") {
  refellipsoidEuropa(Var::refellipsoid(ws), model, Var::verbosity(ws));
}

/*! Conversion of 3D ellipsoid to 1D curvature radius.

Calculates the curvature radius for the given latitude and azimuth
angle, and uses this to set a spherical reference ellipsoid
suitable for 1D calculations. The curvature radius is a better
local approximation than using the local ellipsoid radius.

The used expression assumes a geodetic latitude, but also
latitudes should be OK as using this method anyhow signifies
an approximation.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] latitude - Latitude.
@param[in] azimuth - Azimuth angle.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void refellipsoidForAzimuth(Workspace& ws, const Numeric& latitude,
                            const Numeric& azimuth) {
  refellipsoidForAzimuth(Var::refellipsoid(ws), latitude, azimuth,
                         Var::verbosity(ws));
}

/*! Ganymede reference ellipsoids.

From Wikipedia

@author Takayoshi Yamada

@param[in,out] Workspace ws - An ARTS workspace
@param[in] model - Model ellipsoid to use. Options listed above. (default:
"Sphere")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void refellipsoidGanymede(Workspace& ws, const String& model = "Sphere") {
  refellipsoidGanymede(Var::refellipsoid(ws), model, Var::verbosity(ws));
}

/*! Io reference ellipsoids.

The reference ellipsoid (*refellipsoid*) is set to model Io,
folowing different models. The options are:

   "Sphere" : A spherical planetesimal. The radius is taken from
      report of the IAU/IAG Working Group.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] model - Model ellipsoid to use. Options listed above. (default:
"Sphere")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void refellipsoidIo(Workspace& ws, const String& model = "Sphere") {
  refellipsoidIo(Var::refellipsoid(ws), model, Var::verbosity(ws));
}

/*! Jupiter reference ellipsoids.

The reference ellipsoid (*refellipsoid*) is set to model Jupiter,
folowing different models. The options are:

   "Sphere" : A spherical planet. The radius is taken from a
      report of the IAU/IAG Working Group.

   "Ellipsoid" : A reference ellipsoid with parameters taken from
      a report of the IAU/IAG Working Group.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] model - Model ellipsoid to use. Options listed above. (default:
"Sphere")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void refellipsoidJupiter(Workspace& ws, const String& model = "Sphere") {
  refellipsoidJupiter(Var::refellipsoid(ws), model, Var::verbosity(ws));
}

/*! Mars reference ellipsoids.

The reference ellipsoid (*refellipsoid*) is set to model Mars,
folowing different models. The options are:

   "Sphere" : A spherical planet. The radius is taken from a
      report of the IAU/IAG Working Group.

   "Ellipsoid" : A reference ellipsoid with parameters taken from
      a report of the IAU/IAG Working Group.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] model - Model ellipsoid to use. Options listed above. (default:
"Sphere")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void refellipsoidMars(Workspace& ws, const String& model = "Sphere") {
  refellipsoidMars(Var::refellipsoid(ws), model, Var::verbosity(ws));
}

/*! Moon reference ellipsoids.

The reference ellipsoid (*refellipsoid*) is set to model Moon,
folowing different models. The options are:

   "Sphere" : A spherical planet. The radius is taken from a
      report of the IAU/IAG Working Group.

   "Ellipsoid" : A reference ellipsoid with parameters taken from
      Wikepedia (see code for details). The IAU/IAG working group
      defines the Moon ellipsoid to be a sphere.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] model - Model ellipsoid to use. Options listed above. (default:
"Sphere")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void refellipsoidMoon(Workspace& ws, const String& model = "Sphere") {
  refellipsoidMoon(Var::refellipsoid(ws), model, Var::verbosity(ws));
}

/*! Conversion of 3D ellipsoid to 2D orbit track geometry.

Determines an approximate reference ellipsoid following an orbit
track. The new ellipsoid is determined simply, by determining the
radius at the maximum latitude and from this value calculate a new
new eccentricity. The orbit is specified by giving the orbit
inclination (*orbitinc*), that is normally a value around 100 deg
for polar sun-synchronous orbits.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] orbitinc - Orbit inclination.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void refellipsoidOrbitPlane(Workspace& ws, const Numeric& orbitinc) {
  refellipsoidOrbitPlane(Var::refellipsoid(ws), orbitinc, Var::verbosity(ws));
}

/*! Manual setting of the reference ellipsoid.

The two values of *refellipsoid* can here be set manually. The two
arguments correspond directly to first and second element of
*refellipsoid*.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] re - Average or equatorial radius.
@param[in] e - Eccentricity (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void refellipsoidSet(Workspace& ws, const Numeric& re, const Numeric& e = 0) {
  refellipsoidSet(Var::refellipsoid(ws), re, e, Var::verbosity(ws));
}

/*! Venus reference ellipsoids.

The reference ellipsoid (*refellipsoid*) is set to model Venus,
folowing different models. The options are:

   "Sphere" : A spherical planet. The radius is taken from a
      report of the IAU/IAG Working Group.

According to the report used above, the Venus ellipsoid lacks
eccentricity and no further models should be required.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] model - Model ellipsoid to use. Options listed above. (default:
"Sphere")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void refellipsoidVenus(Workspace& ws, const String& model = "Sphere") {
  refellipsoidVenus(Var::refellipsoid(ws), model, Var::verbosity(ws));
}

/*! Microwave refractive index due to free electrons.

The refractive index of free electrons is added to *refr_index_air*.
To obtain the complete value, *refr_index_air* should be set to 1
before calling this WSM. This applies also to *refr_index_air_group*.

The expression applied is n=sqrt(1-wp^2/w^2) where wp is the plasma
frequency, and w is the angular frequency (the function returns
n-1, that here is slightly negative). This expressions is found in
many textbooks, e.g. Rybicki and Lightman (1979). The above refers
to *refr_index_air*. *refr_index_air_group* is sqrt(1+wp^2/w^2).

The expression is dispersive. The frequency applied is the mean of
first and last element of *f_grid* is selected. This frequency must
be at least twice the plasma frequency.

An error is issued if free electrons not are part of *abs_species*
(and there exist a corresponding "vmr"-value). This demand is
removed if *demand_vmr_value* is set to 0, but use this option
with care.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] demand_vmr_value - Flag to control if it is demanded that free
electrons are in *abs_species*. Default is that this is demanded. (default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void refr_index_airFreeElectrons(Workspace& ws,
                                 const Index& demand_vmr_value = 1) {
  refr_index_airFreeElectrons(Var::refr_index_air(ws),
                              Var::refr_index_air_group(ws), Var::f_grid(ws),
                              Var::abs_species(ws), Var::rtp_vmr(ws),
                              demand_vmr_value, Var::verbosity(ws));
}

/*! Calculates the IR refractive index due to gases in the
Earth's atmosphere.

Only refractivity of dry air is considered. The formula used is
contributed by Michael Hoepfner, Forschungszentrum Karlsruhe.

The refractivity of dry air is added to *refr_index_air*. To obtain
the complete value, *refr_index_air* should be set to 1 before
calling this WSM. This applies also to *refr_index_air_group*.

The expression used is non-dispersive. Hence, *refr_index_air* and
*refr_index_air_group* are identical.

@author Mattias Ekstrom

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void refr_index_airInfraredEarth(Workspace& ws) {
  refr_index_airInfraredEarth(
      Var::refr_index_air(ws), Var::refr_index_air_group(ws),
      Var::rtp_pressure(ws), Var::rtp_temperature(ws), Var::verbosity(ws));
}

/*! Microwave refractive index in Earth's atmosphere.

This method just considers pressure, temperature and water
vapour, which should suffice for Earth. For a more general
method, see *refr_index_airMicrowavesGeneral*.

The refractivity of dry air and water vapour is added to
*refr_index_air*. To obtain the complete value, *refr_index_air*
should be set to 1 before calling this WSM. This applies also to
*refr_index_air_group.

The expression used is non-dispersive. Hence, *refr_index_air*
and *refr_index_air_group* are identical.

The standard expression for Earth and microwaves is used:
   N = k1*(P-e)/T + k2*e/T + k3*e/T^2
where N is refractivity, P is pressure, T is temperature and
e is water vapour partial pressure. The values of k1, k2 and k3
can be modified.

Many different values of k1, k2 and k3 can be found in the
literature. The default values applied here are taken from
Bevis et al., GPS meteorology: Mapping ..., JAM, 1994.
More specifically, these value are found in Table 1, listed
as "Present study". Note that in ARTS Pa is used for pressure
and k1, k2 and k3 must be adjusted accordingly.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] k1 - Coefficient a, see above (default: 77.6e-8)
@param[in] k2 - Coefficient b, see above (default: 70.4e-8)
@param[in] k3 - Coefficient c, see above (default: 3.739e-3)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void refr_index_airMicrowavesEarth(Workspace& ws, const Numeric& k1 = 77.6e-8,
                                   const Numeric& k2 = 70.4e-8,
                                   const Numeric& k3 = 3.739e-3) {
  refr_index_airMicrowavesEarth(
      Var::refr_index_air(ws), Var::refr_index_air_group(ws),
      Var::rtp_pressure(ws), Var::rtp_temperature(ws), Var::rtp_vmr(ws),
      Var::abs_species(ws), k1, k2, k3, Var::verbosity(ws));
}

/*! Microwave refractive index due to gases in planetary atmospheres.

The refractivity of a specified gas mixture is calculated and added
to *refr_index_air*. To obtain the complete value, *refr_index_air*
should be set to 1 before calling this WSM. This applies also to
*refr_index_air_group.

The expression used is non-dispersive. Hence, *refr_index_air* and
*refr_index_air_group* are identical.

Uses the methodology introduced by Newell&Baird (1965) for calculating
refractivity of variable gas mixtures based on refractivity of the
individual gases at reference conditions. Assuming ideal gas law for
converting reference refractivity to actual pressure and temperature
conditions. Reference refractivities are also taken from Newell&Baird (1965)
and are vailable for N2, O2, CO2, H2, and He. Additionally, H2O reference
refractivity has been derived from H2O contribution in Thayer (see
*refr_index_airMicrowavesEarth*) for T0=273.15K. Any mixture of these gases
can be taken into account.

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void refr_index_airMicrowavesGeneral(Workspace& ws) {
  refr_index_airMicrowavesGeneral(
      Var::refr_index_air(ws), Var::refr_index_air_group(ws),
      Var::rtp_pressure(ws), Var::rtp_temperature(ws), Var::rtp_vmr(ws),
      Var::abs_species(ws), Var::verbosity(ws));
}

/*! Adds an absorption species to the retrieval quantities.

Similar to *jacobianAddAbsSpecies* but also sets the corresponding block in
*covmat_sx* to the matrices provided in *covmat_block* and *covmat_inv_block*.
The dimensions of *covmat_block* are required to agree with the dimensions of
the retrieval grid.

*covmat_inv_block* must be either empty or the same dimension as *covmat_block*.
If provided, this matrix will be used as the inverse for the covariance matrix
block and numerical inversion of this block is thus avoided. Note, however, that
this is only effective if this block is uncorrelated with any other retrieval
quantity.

For number and order of elements added to *x*, see *jacobianAddAbsSpecies*.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[in] g1 - Pressure retrieval grid.
@param[in] g2 - Latitude retrieval grid.
@param[in] g3 - Longitude retreival grid.
@param[in] species - The species tag of the retrieval quantity.
@param[in] unit - Retrieval unit. See above. (default: "rel")
@param[in] for_species_tag - Index-bool for acting on species tags or species.
(default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void retrievalAddAbsSpecies(Workspace& ws, const Vector& g1, const Vector& g2,
                            const Vector& g3, const String& species,
                            const String& unit = "rel",
                            const Index& for_species_tag = 1) {
  retrievalAddAbsSpecies(ws, Var::covmat_sx(ws), Var::jacobian_quantities(ws),
                         Var::jacobian_agenda(ws), Var::atmosphere_dim(ws),
                         Var::covmat_block(ws), Var::covmat_inv_block(ws),
                         Var::p_grid(ws), Var::lat_grid(ws), Var::lon_grid(ws),
                         g1, g2, g3, species, unit, for_species_tag,
                         Var::verbosity(ws));
}

/*! Similar to *jacobianAddBasicCatalogParameter* but also adds a corresponding
block to *covmat_sx* with the given *var* as variance value.

For number and order of elements added to *x*,
see *jacobianAddBasicCatalogParameter*.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[in] catalog_identity - The catalog line matching information.
@param[in] catalog_parameter - The catalog parameter of the retrieval quantity.
@param[in] var - The variance of the catalog parameter.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void retrievalAddCatalogParameter(Workspace& ws,
                                  const QuantumIdentifier& catalog_identity,
                                  const String& catalog_parameter,
                                  const Numeric& var) {
  retrievalAddCatalogParameter(ws, Var::covmat_sx(ws),
                               Var::jacobian_quantities(ws),
                               Var::jacobian_agenda(ws), catalog_identity,
                               catalog_parameter, var, Var::verbosity(ws));
}

/*! Same as *jacobianAddBasicCatalogParameters* but also adds a new
block to *covmat_sx* using the matrices in *covmat_block* and
*covmat_inv_block*.

If *covmat_inv_block* is non-empty, it is used as inverse for the added block
which avoids its numerical computation.

For number and order of elements added to *x*,
see *jacobianAddBasicCatalogParameters*.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[in] catalog_identities - The catalog line matching informations.
@param[in] catalog_parameters - The catalog parameters of the retrieval
quantity.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void retrievalAddCatalogParameters(
    Workspace& ws, const ArrayOfQuantumIdentifier& catalog_identities,
    const ArrayOfString& catalog_parameters) {
  retrievalAddCatalogParameters(ws, Var::covmat_sx(ws),
                                Var::jacobian_quantities(ws),
                                Var::jacobian_agenda(ws), Var::covmat_block(ws),
                                Var::covmat_inv_block(ws), catalog_identities,
                                catalog_parameters, Var::verbosity(ws));
}

/*! Same as *jacobianAddFreqShift* but also adds the correlation block
contained in *covmat_block* and *covmat_inv_block* to *covmat_sx*.

For number and order of elements added to *x*, see *jacobianAddFreqShift*.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[in] df - Size of perturbation to apply. (default: 100e3)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void retrievalAddFreqShift(Workspace& ws, const Numeric& df = 100e3) {
  retrievalAddFreqShift(ws, Var::covmat_sx(ws), Var::jacobian_quantities(ws),
                        Var::jacobian_agenda(ws), Var::covmat_block(ws),
                        Var::covmat_inv_block(ws), Var::f_grid(ws), df,
                        Var::verbosity(ws));
}

/*! Same as *jacobianAddFreqShift* but also adds the correlation block
contained in *covmat_block* and *covmat_inv_block* to *covmat_sx*.

For number and order of elements added to *x*, see *jacobianAddFreqStretch*.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[in] df - Size of perturbation to apply. (default: 100e3)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void retrievalAddFreqStretch(Workspace& ws, const Numeric& df = 100e3) {
  retrievalAddFreqStretch(ws, Var::covmat_sx(ws), Var::jacobian_quantities(ws),
                          Var::jacobian_agenda(ws), Var::f_grid(ws),
                          Var::covmat_block(ws), Var::covmat_inv_block(ws), df,
                          Var::verbosity(ws));
}

/*! Same as *jacobianAddMagField* but also adds a new block to *covmat_sx*
using the matrices in *covmat_block* and *covmat_inv_block*.

If *covmat_inv_block* is non-empty, it is used as inverse for the added block
which avoids its numerical computation.

For number and order of elements added to *x*, see *jacobianAddMagField*.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[in] g1 - Pressure retrieval grid.
@param[in] g2 - Latitude retrieval grid.
@param[in] g3 - Longitude retreival grid.
@param[in] component - Magnetic field component to retrieve (default: "v")
@param[in] dB - Magnetic field perturbation (default: 1.0e-7)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void retrievalAddMagField(Workspace& ws, const Vector& g1, const Vector& g2,
                          const Vector& g3, const String& component = "v",
                          const Numeric& dB = 1.0e-7) {
  retrievalAddMagField(ws, Var::covmat_sx(ws), Var::jacobian_quantities(ws),
                       Var::jacobian_agenda(ws), Var::atmosphere_dim(ws),
                       Var::covmat_block(ws), Var::covmat_inv_block(ws),
                       Var::p_grid(ws), Var::lat_grid(ws), Var::lon_grid(ws),
                       g1, g2, g3, component, dB, Var::verbosity(ws));
}

/*! Same as *jacobianAddPointingZa* but also adds a new block to *covmat_sx*
using the matrices in *covmat_block* and *covmat_inv_block*.

If *covmat_inv_block* is non-empty, it is used as inverse for the added block
which avoids its numerical computation.

For number and order of elements added to *x*, see *jacobianAddPointingZa*.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[in] poly_order - Order of polynomial to describe the time variation of
pointing off-sets. (default: 0)
@param[in] calcmode - Calculation method. See above (default: "recalc")
@param[in] dza - Size of perturbation to apply (when applicable). (default:
0.01)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void retrievalAddPointingZa(Workspace& ws, const Index& poly_order = 0,
                            const String& calcmode = "recalc",
                            const Numeric& dza = 0.01) {
  retrievalAddPointingZa(ws, Var::covmat_sx(ws), Var::jacobian_quantities(ws),
                         Var::jacobian_agenda(ws), Var::covmat_block(ws),
                         Var::covmat_inv_block(ws), Var::sensor_pos(ws),
                         Var::sensor_time(ws), poly_order, calcmode, dza,
                         Var::verbosity(ws));
}

/*! Same as *jacobianAddPolyfit* but also adds a new block to *covmat_sx*
using the matrices in *covmat_block* and *covmat_inv_block*.

If *covmat_inv_block* is non-empty, it is used as inverse for the added block
which avoids its numerical computation.

For number and order of elements added to *x*, see *jacobianAddPolyfit*.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[in] poly_order - Polynomial order to use for the fit.
@param[in] no_pol_variation - Set to 1 if the baseline off-set is the same for
all Stokes components. (default: 0)
@param[in] no_los_variation - Set to 1 if the baseline off-set is the same for
all line-of-sights (inside each measurement block). (default: 0)
@param[in] no_mblock_variation - Set to 1 if the baseline off-set is the same
for all measurement blocks. (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void retrievalAddPolyfit(Workspace& ws, const Index& poly_order,
                         const Index& no_pol_variation = 0,
                         const Index& no_los_variation = 0,
                         const Index& no_mblock_variation = 0) {
  retrievalAddPolyfit(ws, Var::covmat_sx(ws), Var::jacobian_quantities(ws),
                      Var::jacobian_agenda(ws), Var::covmat_block(ws),
                      Var::covmat_inv_block(ws),
                      Var::sensor_response_pol_grid(ws),
                      Var::sensor_response_dlos_grid(ws), Var::sensor_pos(ws),
                      poly_order, no_pol_variation, no_los_variation,
                      no_mblock_variation, Var::verbosity(ws));
}

/*! Same as *jacobianAddPolyfit* but also adds a new block to *covmat_sx*
using the matrices in *covmat_block* and *covmat_inv_block*.

If *covmat_inv_block* is non-empty, it is used as inverse for the added block
which avoids its numerical computation.

For number and order of elements added to *x*, see *jacobianAddScatSpecies*.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[in] g1 - Pressure retrieval grid.
@param[in] g2 - Latitude retrieval grid.
@param[in] g3 - Longitude retreival grid.
@param[in] species - Name of scattering species, must match one element in
*scat_species*.
@param[in] quantity - Retrieval quantity, e.g. "IWC".

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void retrievalAddScatSpecies(Workspace& ws, const Vector& g1, const Vector& g2,
                             const Vector& g3, const String& species,
                             const String& quantity) {
  retrievalAddScatSpecies(ws, Var::covmat_sx(ws), Var::jacobian_quantities(ws),
                          Var::jacobian_agenda(ws), Var::atmosphere_dim(ws),
                          Var::covmat_block(ws), Var::covmat_inv_block(ws),
                          Var::p_grid(ws), Var::lat_grid(ws), Var::lon_grid(ws),
                          g1, g2, g3, species, quantity, Var::verbosity(ws));
}

/*! Same as *jacobianAddSinefit* but also adds a new block to *covmat_sx*
using the matrices in *covmat_block* and *covmat_inv_block*.

If *covmat_inv_block* is non-empty, it is used as inverse for the added block
which avoids its numerical computation.

For number and order of elements added to *x*, see *jacobianAddSinefit*.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[in] period_lengths - Period lengths of the fit.
@param[in] no_pol_variation - Set to 1 if the baseline off-set is the same for
all Stokes components. (default: 0)
@param[in] no_los_variation - Set to 1 if the baseline off-set is the same for
all line-of-sights (inside each measurement block). (default: 0)
@param[in] no_mblock_variation - Set to 1 if the baseline off-set is the same
for all measurement blocks. (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void retrievalAddSinefit(Workspace& ws, const Vector& period_lengths,
                         const Index& no_pol_variation = 0,
                         const Index& no_los_variation = 0,
                         const Index& no_mblock_variation = 0) {
  retrievalAddSinefit(ws, Var::covmat_sx(ws), Var::jacobian_quantities(ws),
                      Var::jacobian_agenda(ws), Var::covmat_block(ws),
                      Var::covmat_inv_block(ws),
                      Var::sensor_response_pol_grid(ws),
                      Var::sensor_response_dlos_grid(ws), Var::sensor_pos(ws),
                      period_lengths, no_pol_variation, no_los_variation,
                      no_mblock_variation, Var::verbosity(ws));
}

/*! Same as *jacobianAddSpecialSpecies* but also adds a new block to *covmat_sx*
using the matrices in *covmat_block* and *covmat_inv_block*.

If *covmat_inv_block* is non-empty, it is used as inverse for the added block
which avoids its numerical computation.

For number and order of elements added to *x*, see *jacobianAddSpecialSpecies*.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[in] g1 - Pressure retrieval grid.
@param[in] g2 - Latitude retrieval grid.
@param[in] g3 - Longitude retreival grid.
@param[in] species - The species of the retrieval quantity.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void retrievalAddSpecialSpecies(Workspace& ws, const Vector& g1,
                                const Vector& g2, const Vector& g3,
                                const String& species) {
  retrievalAddSpecialSpecies(
      ws, Var::covmat_sx(ws), Var::jacobian_quantities(ws),
      Var::jacobian_agenda(ws), Var::atmosphere_dim(ws), Var::covmat_block(ws),
      Var::covmat_inv_block(ws), Var::p_grid(ws), Var::lat_grid(ws),
      Var::lon_grid(ws), g1, g2, g3, species, Var::verbosity(ws));
}

/*! Same as *jacobianAddSurfaceQuantity* but also adds a new block to
*covmat_sx* using the matrices in *covmat_block* and *covmat_inv_block*.

If *covmat_inv_block* is non-empty, it is used as inverse for the added block
which avoids its numerical computation.

For number and order of elements added to *x*, see *jacobianAddSurfaceQuantity*.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] g1 - Latitude retrieval grid.
@param[in] g2 - Longitude retreival grid.
@param[in] quantity - Retrieval quantity, e.g. "Wind speed".

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void retrievalAddSurfaceQuantity(Workspace& ws, const Vector& g1,
                                 const Vector& g2, const String& quantity) {
  retrievalAddSurfaceQuantity(
      ws, Var::covmat_sx(ws), Var::jacobian_quantities(ws),
      Var::jacobian_agenda(ws), Var::covmat_block(ws),
      Var::covmat_inv_block(ws), Var::atmosphere_dim(ws), Var::lat_grid(ws),
      Var::lon_grid(ws), g1, g2, quantity, Var::verbosity(ws));
}

/*! Same as *jacobianAddTemperature* but also adds a new block to *covmat_sx*
using the matrices in *covmat_block* and *covmat_inv_block*.

If *covmat_inv_block* is non-empty, it is used as inverse for the added block
which avoids its numerical computation.

For number and order of elements added to *x*, see *jacobianAddTemperature*.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[in] g1 - Pressure retrieval grid.
@param[in] g2 - Latitude retrieval grid.
@param[in] g3 - Longitude retreival grid.
@param[in] hse - Flag to assume HSE or not ("on" or "off"). (default: "on")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void retrievalAddTemperature(Workspace& ws, const Vector& g1, const Vector& g2,
                             const Vector& g3, const String& hse = "on") {
  retrievalAddTemperature(ws, Var::covmat_sx(ws), Var::jacobian_quantities(ws),
                          Var::jacobian_agenda(ws), Var::atmosphere_dim(ws),
                          Var::covmat_block(ws), Var::covmat_inv_block(ws),
                          Var::p_grid(ws), Var::lat_grid(ws), Var::lon_grid(ws),
                          g1, g2, g3, hse, Var::verbosity(ws));
}

/*! Same as *jacobianAddWind* but also adds a new block to *covmat_sx*
using the matrices in *covmat_block* and *covmat_inv_block*.

If *covmat_inv_block* is non-empty, it is used as inverse for the added block
which avoids its numerical computation.

For number and order of elements added to *x*, see *jacobianAddWind*.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[in] g1 - Pressure retrieval grid.
@param[in] g2 - Latitude retrieval grid.
@param[in] g3 - Longitude retrieval grid.
@param[in] component - Wind component to retrieve (default: "v")
@param[in] dfrequency - This is the frequency perturbation (default: 0.1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void retrievalAddWind(Workspace& ws, const Vector& g1, const Vector& g2,
                      const Vector& g3, const String& component = "v",
                      const Numeric& dfrequency = 0.1) {
  retrievalAddWind(ws, Var::covmat_sx(ws), Var::jacobian_quantities(ws),
                   Var::jacobian_agenda(ws), Var::atmosphere_dim(ws),
                   Var::covmat_block(ws), Var::covmat_inv_block(ws),
                   Var::p_grid(ws), Var::lat_grid(ws), Var::lon_grid(ws), g1,
                   g2, g3, component, dfrequency, Var::verbosity(ws));
}

/*! Closes the definition of retrieval quantities and correlations and
prepares related WSVs for the retrieval.

This function calls jacobianClose and checks that the corvariance matrices
are consistent with the Jacobian.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void retrievalDefClose(Workspace& ws) {
  retrievalDefClose(ws, Var::jacobian_do(ws), Var::jacobian_agenda(ws),
                    Var::retrieval_checked(ws), Var::covmat_sx(ws),
                    Var::jacobian_quantities(ws), Var::verbosity(ws));
}

/*! Begin retrieval definition section.

This function initialises all variables required for defining
retrieval quantities and corresponding covariance matrices.
By default, Jacobian quantities should be added withing the.
retrieval definition section. If Jacobian quantities are
defined separately *initialize_jacobian* must be set to 0,
otherwise the quantities will be discarded.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[in] initialize_jacobian - Flag whether or not to (re)initialize
Jacobian-related quantities. Set to 0 if Jacobian is already defined. (default:
1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void retrievalDefInit(Workspace& ws, const Index& initialize_jacobian = 1) {
  retrievalDefInit(Var::covmat_se(ws), Var::covmat_sx(ws),
                   Var::covmat_block(ws), Var::covmat_inv_block(ws),
                   Var::jacobian_quantities(ws), Var::jacobian_agenda(ws),
                   initialize_jacobian, Var::verbosity(ws));
}

/*! Extract retrieval error from covariance matrices.

Extracts the error estimates for the retrieved quantities from the covariance
matrices for the error due to measurement noise *covmat_so* and the error due
to limited resolution of the observation system *covmat_ss* and stores them in
the vectors *retrieval_eo* and *retrieval_ss*, respectively.
To etract these errors, first the convariance matrices of which the errors
should be extracted have to be computed using the WSMs *covmat_soCalc*
and *covmat_ssCalc* or set to be empty in order to be ignored. Note, however,
that this will also set the corresponding error vector to be empty.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void retrievalErrorsExtract(Workspace& ws) {
  retrievalErrorsExtract(Var::retrieval_eo(ws), Var::retrieval_ss(ws),
                         Var::covmat_so(ws), Var::covmat_ss(ws),
                         Var::verbosity(ws));
}

/*! The geometric line-of-sight between two points.

The method sets *rte_los* to the line-of-sight, at *rte_pos*,
that matches the geometrical propagation path between *rte_pos*
and *rte_pos2*.

The standard case should be that *rte_pos2* corresponds to a
transmitter, and *rte_pos* to the receiver/sensor.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void rte_losGeometricFromRtePosToRtePos2(Workspace& ws) {
  rte_losGeometricFromRtePosToRtePos2(Var::rte_los(ws), Var::atmosphere_dim(ws),
                                      Var::lat_grid(ws), Var::lon_grid(ws),
                                      Var::refellipsoid(ws), Var::rte_pos(ws),
                                      Var::rte_pos2(ws), Var::verbosity(ws));
}

/*! Sets *rte_los* to the given angles.

The azimuth angle is ignored for 1D and 2D.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] za - Zenith angle of sensor line-of-sight.
@param[in] aa - Azimuth angle of sensor line-of-sight.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void rte_losSet(Workspace& ws, const Numeric& za, const Numeric& aa) {
  rte_losSet(Var::rte_los(ws), Var::atmosphere_dim(ws), za, aa,
             Var::verbosity(ws));
}

/*! Sets *rte_pos* to the given co-ordinates.

The longitude is ignored for 1D and 2D, and the latitude is also
ignored for 1D.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] z - Geometrical altitude of sensor position.
@param[in] lat - Latitude of sensor position.
@param[in] lon - Longitude of sensor position.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void rte_posSet(Workspace& ws, const Numeric& z, const Numeric& lat,
                const Numeric& lon) {
  rte_posSet(Var::rte_pos(ws), Var::atmosphere_dim(ws), z, lat, lon,
             Var::verbosity(ws));
}

/*! Sets *rte_pos* and *rte_los* to values for last point in *ppath*.

For example, if the propagation path intersects with the surface,
this method gives you the position and angle of *ppath* at the
surface.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void rte_pos_losMoveToStartOfPpath(Workspace& ws) {
  rte_pos_losMoveToStartOfPpath(Var::rte_pos(ws), Var::rte_los(ws),
                                Var::atmosphere_dim(ws), Var::ppath(ws),
                                Var::verbosity(ws));
}

/*! Sets NLTE values manually

Touch

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] data - Vibrational data [nlevels]

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void rtp_nlteFromRaw(Workspace& ws, const Vector& data) {
  rtp_nlteFromRaw(Var::rtp_nlte(ws), Var::nlte_level_identifiers(ws),
                  Var::nlte_vibrational_energies(ws), data, Var::verbosity(ws));
}

/*! Prepares *scat_data* for the scattering solver.

Derives single scattering data for the frequencies given by
*f_grid* by interpolation from *scat_data_raw*. *f_grid* should be
the actual WSV *f_grid* or a single-element Vector.

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] interp_order - Interpolation order. (default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void scat_dataCalc(Workspace& ws, const Index& interp_order = 1) {
  scat_dataCalc(Var::scat_data(ws), Var::scat_data_raw(ws), Var::f_grid(ws),
                interp_order, Var::verbosity(ws));
}

/*! Method for checking the validity and consistency of the single
scattering properties in *scat_data*.

It checks that *scat_data* does not contain any invalid values,
that is any NaN elements in K, Z, or a or any negative values in
the 'scalar' properties K11, Z11, and a1.

When *check_type* is 'all', it is furthermore checked that the
scattering matrix is properly normalized, that is that the solid
sphere integrated scattering matrix (int_Z11), which is supposed to
be normalized to the scattering cross section, is sufficiently
consistent with the scattering cross section (C_sca) derived from
the difference of extinction (K11) and absorption (a1):
int_z11 ~ C_sca = K11-a1.
Sufficient consistency is defined by the maximum allowed deviation
in single scattering albedo, *sca_mat_threshold*, testing for
  ( <int_Z11>/<C_sca>-1. ) * ( <C_sca>/<K11> ) <= sca_mat_threshold.
The check is skipped if *check_type* is 'sane'.

@author Claudia Emde
@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] check_type - The level of checks to apply on scat_data ('sane' or
'all'; see above). (default: "all")
@param[in] sca_mat_threshold - Threshold for allowed albedo deviation (see
above). (default: 5e-2)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void scat_dataCheck(Workspace& ws, const String& check_type = "all",
                    const Numeric& sca_mat_threshold = 5e-2) {
  scat_dataCheck(Var::scat_data(ws), check_type, sca_mat_threshold,
                 Var::verbosity(ws));
}

/*! Reduces temperature dimension of single scattering to a single entry.

FIXME...
Derives single scattering data for the frequencies given by
*f_grid* by interpolation from *scat_data*. *f_grid* should be
the actual WSV *f_grid* or a single-element Vector.

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] scat_index - Apply on *scat_data* from scattering species of this
index (0-based).
@param[in] temperature - Temperature to interpolate *scat_data* to.
@param[in] interp_order - Interpolation order. (default: 1)
@param[in] phamat_only - Flag whether to apply temperture reduction on phase
matrix data only (1) or on all single scattering properties (0). (default: 1)
@param[in] sca_mat_threshold - Threshold for allowed albedo deviation. (default:
5e-2)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void scat_dataReduceT(Workspace& ws, const Index& scat_index,
                      const Numeric& temperature, const Index& interp_order = 1,
                      const Index& phamat_only = 1,
                      const Numeric& sca_mat_threshold = 5e-2) {
  scat_dataReduceT(Var::scat_data(ws), scat_index, temperature, interp_order,
                   phamat_only, sca_mat_threshold, Var::verbosity(ws));
}

/*! Checks dimensions, grids and single scattering properties of all
scattering elements in *scat_data*.

Dimension and grid equirements:
- The scattering element's f_grid is either identical to *f_grid* or
  of dimension 1.
- In the latter case, the scattering element's f_grid value must
  not deviate from any of the *f_grid* values by more than a
  fraction of *dfrel_threshold*.
- The frequency dimension of pha_mat_data, ext_mat_data, and
  abs_vec_data is either equal to the scattering element's f_grid
  or 1.
- The temperature dimension of pha_mat_data, ext_mat_data, and
  abs_vec_data is either equal to the scattering element's T_grid
  or 1.
- The temperature dimension of ext_mat_data, and abs_vec_data is
  identical.

The single scattering property contents are checked using
*scat_dataCheck*. For details, see there. The depth of these checks
and their rigour can adapted (see description of parameters
*check_level* and *sca_mat_threshold* in *scat_dataCheck*) or can
be skipped entirely (setting *check_level* to 'none').
NOTE: These test shall only be skipped when one is confident that
the data is correct, e.g. by having run *scat_dataCheck* on the set
of data before, e.g. in a separate ARTS run.

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace
@param[in] dfrel_threshold - Maximum relative frequency deviation between
(single entry) scattering element f_grid values and the RT calculation's
*f_grid*. (default: 0.1)
@param[in] check_level - See *check_level* in *scat_dataCheck*. (default: "all")
@param[in] sca_mat_threshold - See *sca_mat_threshold* in *scat_dataCheck*.
(default: 5e-2)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void scat_data_checkedCalc(Workspace& ws, const Numeric& dfrel_threshold = 0.1,
                           const String& check_level = "all",
                           const Numeric& sca_mat_threshold = 5e-2) {
  scat_data_checkedCalc(Var::scat_data_checked(ws), Var::scat_data(ws),
                        Var::f_grid(ws), dfrel_threshold, check_level,
                        sca_mat_threshold, Var::verbosity(ws));
}

/*! Interpolates *scat_data* by frequency to give *scat_data_mono*.

@author Cory Davis

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void scat_data_monoCalc(Workspace& ws) {
  scat_data_monoCalc(Var::scat_data_mono(ws), Var::scat_data(ws),
                     Var::f_grid(ws), Var::f_index(ws), Var::verbosity(ws));
}

/*! Extracts data at *f_index* from *scat_data* to give *scat_data_mono*.

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void scat_data_monoExtract(Workspace& ws) {
  scat_data_monoExtract(Var::scat_data_mono(ws), Var::scat_data(ws),
                        Var::f_index(ws), Var::verbosity(ws));
}

/*! A basic interface to Mishchenko's T-matrix code linked to ARTS.

The method performs a T-matrix calculation for a single scattering
element, i.e. a combination of particle shape, size, aspect ratio
and orientation.

Particle shape (*shape*) has two options:
   "spheroidal" and "cylindrical"

Particle size (*diameter_volume_equ*) is given as the equivalent
volume sphere diameter. That is, the diameter obtained if all the
particle's material is rearranged into a (solid) sphere.

Particle aspect ratio ar (*aspect_ratio*) is a numeric value, defined
according to Mishchenko's definition as ratio of horizontal axis a to
vertical (rotational) axis b: ar=a/b. That is, oblates have ar>1,
prolates ar<1.
Perfect spheres (spheroidals with ar=1) can trigger numerical issues.
To avoid these, we internally increase their aspect ratio by 1e-6,
i.e. turning perfect spheres into very light oblates.

Particle type (*ptype*) has two options:
   "totally_random" and "azimuthally_random"
For totally randomly oriented particles, *data_aa_grid* is not taken
into account (but a Vector type container needs to be passed).

For further information on how aspect ratio and the different shapes
and orientations are defined, see the documentation of the T-matrix
code found http://www.giss.nasa.gov/staff/mmishchenko/t_matrix.html

Regarding *ndgs*, we refer to the this comment from the documentation:
   "Parameter controlling the number of division points
   in computing integrals over the particle surface.
   For compact particles, the recommended value is 2.
   For highly aspherical particles larger values (3, 4,...)
   may be necessary to obtain convergence.
   The code does not check convergence over this parameter.
   Therefore, control comparisons of results obtained with
   different NDGS-values are recommended."

@author Johan Strandgren
@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] shape - Particle shape. Options listed above.
@param[in] diameter_volume_equ - Particle volume equivalent diameter [m]. See
defintion above.
@param[in] aspect_ratio - Particle aspect ratio.
@param[in] mass - Particle mass. This information is just included in the meta
data, and does not affect the T-matrix calculations. (default:
std::numeric_limits<Numeric>::quiet_NaN())
@param[in] ptype - Particle type/orientation. Options listed above.
@param[in] data_f_grid - Frequency grid of the scattering data to be calculated.
@param[in] data_t_grid - Temperature grid of the scattering data to be
calculated.
@param[in] data_za_grid - Zenith angle grid of the scattering data to be
calculated.
@param[in] data_aa_grid - Azimuth angle grid of the scattering data to be
calculated. (default: {})
@param[in] precision - Accuracy of the computations. (default: 0.001)
@param[in] cri_source - String describing the source of *complex_refr_index*,
for inclusion in meta data. (default: "Set by user, unknown source.")
@param[in] ndgs - See above. So far only applied for random orientation.
(default: 2)
@param[in] robust - Continue even if individual T-matrix calculations fail.
Respective scattering element data will be NAN. (default: 0)
@param[in] quiet - Suppress print output from tmatrix fortran code. (default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void scat_data_singleTmatrix(
    Workspace& ws, const String& shape, const Numeric& diameter_volume_equ,
    const Numeric& aspect_ratio, const String& ptype, const Vector& data_f_grid,
    const Vector& data_t_grid, const Vector& data_za_grid,
    const Numeric& mass = std::numeric_limits<Numeric>::quiet_NaN(),
    const Vector& data_aa_grid = {}, const Numeric& precision = 0.001,
    const String& cri_source = "Set by user, unknown source.",
    const Index& ndgs = 2, const Index& robust = 0, const Index& quiet = 1) {
  scat_data_singleTmatrix(
      Var::scat_data_single(ws), Var::scat_meta_single(ws),
      Var::complex_refr_index(ws), shape, diameter_volume_equ, aspect_ratio,
      mass, ptype, data_f_grid, data_t_grid, data_za_grid, data_aa_grid,
      precision, cri_source, ndgs, robust, quiet, Var::verbosity(ws));
}

/*! Sets sensor WSVs to obtain monochromatic pencil beam values.

The variables are set as follows:
   mblock_dlos_grid        : One row with zero(s).
   sensor_response*        : As returned by *sensor_responseInit*.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void sensorOff(Workspace& ws) {
  sensorOff(Var::sensor_response(ws), Var::sensor_response_f(ws),
            Var::sensor_response_pol(ws), Var::sensor_response_dlos(ws),
            Var::sensor_response_f_grid(ws), Var::sensor_response_pol_grid(ws),
            Var::sensor_response_dlos_grid(ws), Var::mblock_dlos_grid(ws),
            Var::stokes_dim(ws), Var::f_grid(ws), Var::verbosity(ws));
}

/*! Checks consistency of the sensor variables.

The following WSVs are examined: *f_grid*, *sensor_pos*, *sensor_los*,
*transmitter_pos*, *mblock_dlos_grid*, *antenna_dim*,
*sensor_response*, *sensor_response_f*, *sensor_response_pol*,
and *sensor_response_dlos*.

If any of these variables are changed, then this method shall be
called again (no automatic check that this is fulfilled!).

The main tests are that dimensions of sensor variables agree
with other settings, e.g., the size of f_grid, atmosphere_dim,
stokes_dim, etc.

If any test fails, there is an error. Otherwise, *sensor_checked*
is set to 1.

@author Jana Mendrok

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void sensor_checkedCalc(Workspace& ws) {
  sensor_checkedCalc(Var::sensor_checked(ws), Var::atmosphere_dim(ws),
                     Var::stokes_dim(ws), Var::f_grid(ws), Var::sensor_pos(ws),
                     Var::sensor_los(ws), Var::transmitter_pos(ws),
                     Var::mblock_dlos_grid(ws), Var::sensor_response(ws),
                     Var::sensor_response_f(ws), Var::sensor_response_pol(ws),
                     Var::sensor_response_dlos(ws), Var::verbosity(ws));
}

/*! The geometric line-of-sight between pair of points.

The method sets *sensor_los* to the line-of-sights, that matches the
geometrical propagation path from *sensor_pos* to *target_pos*. This
is done for pair of positions, i.e. the two matrices shall have the same
number of rows. The number of columns in *target_pos* shall be two for
1D and 2D and two for 3D, exactly as for *rte_pos2*.

See also *rte_losGeometricFromRtePosToRtePos2*. This method calls that
method for each pair of positions, where values in *sensor_pos* matches
*rte_pos and values in *target_pos* matches *rte_pos2*.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] target_pos - Target position, for each position in *sensor_pos*.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void sensor_losGeometricFromSensorPosToOtherPositions(
    Workspace& ws, const Matrix& target_pos) {
  sensor_losGeometricFromSensorPosToOtherPositions(
      Var::sensor_los(ws), Var::atmosphere_dim(ws), Var::lat_grid(ws),
      Var::lon_grid(ws), Var::refellipsoid(ws), Var::sensor_pos(ws), target_pos,
      Var::verbosity(ws));
}

/*! Includes response of the antenna.

The function returns the sensor response matrix after the antenna
characteristics have been included.

The function handles "multi-beam" cases where the polarisation
coordinate system is the same for all beams.

See *antenna_dim*, *antenna_dlos* and *antenna_response* for
details on how to specify the antenna response.

The text below refers to *mblock_dlos_grid* despite it is not an
input to the method. The method instead uses *sensor_response_dlos_grid*
but the values in this WSV are likely coming from *mblock_dlos_grid*.

One dimensional antenna patterns are handled as other response
functions. That is, both antenna response and radiances are treated
as piece-wise linear functions, and the pencil beam calculations must
cover the full sensor response (i.e. *mblock_dlos_grid* must be
sufficiently broad).

There exist different options for two dimensional (2D) antenna patterns,
see below (if 2D, the GIN *option_2d* must be set, the default results
in an error). A normalisation is always applied for 2D antennas (i.e.
*sensor-norm* is ignored).

"interp_response"For this option, each direction defined by *mblock_dlos_grid*
is considered to represent the same size in terms of solid beam angle, and the
antenna pattern is interpolated to these directions. There is no check on how
well *mblock_dlos_grid* covers the antenna response. The response is treated to
be zero outside the ranges of its  anular grids

"gridded_dlos"This option is more similar to the 1D case. The radiances are
treated as a bi-linear function, but the antenna response is treated as step-
wise constant function (in contrast to 1D). For this option
*mblock_dlos_grid* must match a combination of zenith and azimuth
grids, and this for a particular order. If the zenith and azimuth
grids have 3 and 2 values, respectively, the order shall be:
  [(za1,aa1); (za2,aa1); (za3,aa1); (za1,aa2); (za2,aa2); (za3,aa2) ]
Both these grids must be strictly increasing and as for 1D must cover
the antenna response completely.

@author Patrick Eriksson
@author Mattias Ekstrom

@param[in,out] Workspace ws - An ARTS workspace
@param[in] option_2d - Calculation option for 2D antenna cases. See above for
details. (default: "-")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void sensor_responseAntenna(Workspace& ws, const String& option_2d = "-") {
  sensor_responseAntenna(
      Var::sensor_response(ws), Var::sensor_response_f(ws),
      Var::sensor_response_pol(ws), Var::sensor_response_dlos(ws),
      Var::sensor_response_dlos_grid(ws), Var::sensor_response_f_grid(ws),
      Var::sensor_response_pol_grid(ws), Var::atmosphere_dim(ws),
      Var::antenna_dim(ws), Var::antenna_dlos(ws), Var::antenna_response(ws),
      Var::sensor_norm(ws), option_2d, Var::verbosity(ws));
}

/*! Includes response of the backend (spectrometer).

The function returns the sensor response matrix after the backend
characteristics have been included.

See *f_backend*, *backend_channel_response* and *sensor_norm* for
details on how to specify the backend response.

@author Mattias Ekstrom
@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void sensor_responseBackend(Workspace& ws) {
  sensor_responseBackend(
      Var::sensor_response(ws), Var::sensor_response_f(ws),
      Var::sensor_response_pol(ws), Var::sensor_response_dlos(ws),
      Var::sensor_response_f_grid(ws), Var::sensor_response_pol_grid(ws),
      Var::sensor_response_dlos_grid(ws), Var::f_backend(ws),
      Var::backend_channel_response(ws), Var::sensor_norm(ws),
      Var::verbosity(ws));
}

/*! Frequency switching for a pure SSB reciever.

This function can be used for simulation of frequency switching.
That is, when the final spectrum is the difference of two spectra
shifted in frequency. The switching is performed by the LO, but
for a pure singel sideband reciever this is most easily simulated
by instead shifting the backend, as done here.

A strightforward frequency switching is modelled (no folding)
The channel positions for the first measurement cycle are
f_backend+df1, and for the second f_backend+df2. The first
measurement cycle is given the negive weight. That is, the output
is the spectrum for cycle2 minus the spectrum for cycle1.
Output frequency grids are set to *f_backend*.

Use *sensor_responseFrequencySwitching* for double sideband cases.

The method has the same general functionality as, and can replace,
*sensor_responseBackend*.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] df1 - Frequency throw for cycle1.
@param[in] df2 - Frequency throw for cycle2.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void sensor_responseBackendFrequencySwitching(Workspace& ws, const Numeric& df1,
                                              const Numeric& df2) {
  sensor_responseBackendFrequencySwitching(
      Var::sensor_response(ws), Var::sensor_response_f(ws),
      Var::sensor_response_pol(ws), Var::sensor_response_dlos(ws),
      Var::sensor_response_f_grid(ws), Var::sensor_response_pol_grid(ws),
      Var::sensor_response_dlos_grid(ws), Var::f_backend(ws),
      Var::backend_channel_response(ws), Var::sensor_norm(ws), df1, df2,
      Var::verbosity(ws));
}

/*! Simulation of "beam switching".

The measurement procedure is based on taking the difference between
two spectra measured in different directions, and the calculation
set-up must treat exactly two observation directions.

The returned spectrum is y = w1*y + w2*y2, where y1 and w1 are the
spectrum and weight for the first direction, respectively (y2 and
(w2 defined correspondingly for the second direction).

Zenith and azimuth angles after beam switching are set to the
values of the second direction.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] w1 - Weight for values from first viewing direction. (default: -1)
@param[in] w2 - Weight for values from second viewing direction. (default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void sensor_responseBeamSwitching(Workspace& ws, const Numeric& w1 = -1,
                                  const Numeric& w2 = 1) {
  sensor_responseBeamSwitching(
      Var::sensor_response(ws), Var::sensor_response_f(ws),
      Var::sensor_response_pol(ws), Var::sensor_response_dlos(ws),
      Var::sensor_response_dlos_grid(ws), Var::sensor_response_f_grid(ws),
      Var::sensor_response_pol_grid(ws), w1, w2, Var::verbosity(ws));
}

/*! Polynomial frequency interpolation of spectra.

The sensor response methods treat the spectra to be piece-wise linear
functions. This method is a workaround for making methods handling
the spectra in a more elaborate way: it generates spectra on a more
dense grid by polynomial interpolation. The interpolation is not
done explicitly, it is incorporated into *sensor_response*.

This method should in general increase the calculation accuracy for
a given *f_grid*. However, the selection of (original) grid points
becomes more sensitive when using this method. A poor choice of grid
points can result in a decreased accuracy, or generation of negative
radiances. Test calculations indicated that the error easily can
increase with this method close the edge of *f_grid*, and it could
be wise to make *f_grid* a bit wider than actually necessary to avoid
this effect

The method shall be inserted before the antenna stage. That is, this
method shall normally be called directly after *sensor_responseInit*.

Between each neighbouring points of *f_grid*, this method adds
*nfill* grid points. The polynomial order of the interpolation is
*polyorder*.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] polyorder - Polynomial order of interpolation (default: 3)
@param[in] nfill - Number of points to insert in each gap of f_grid (default: 2)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void sensor_responseFillFgrid(Workspace& ws, const Index& polyorder = 3,
                              const Index& nfill = 2) {
  sensor_responseFillFgrid(
      Var::sensor_response(ws), Var::sensor_response_f(ws),
      Var::sensor_response_pol(ws), Var::sensor_response_dlos(ws),
      Var::sensor_response_f_grid(ws), Var::sensor_response_pol_grid(ws),
      Var::sensor_response_dlos_grid(ws), polyorder, nfill, Var::verbosity(ws));
}

/*! Simulation of "frequency switching".

A general method for frequency switching. The WSM
*sensor_responseBackendFrequencySwitching* gives a description of
this observation technique, and is also a more straightforward
 method for pure singel sideband cases.

It is here assume that *sensor_responseMultiMixerBackend* has been
used to calculate the spectrum for two LO positions. This method
calculates the difference between these two spectra, where the
second spectrum gets weight 1 and the first weight -1 (as in
*sensor_responseBackendFrequencySwitching*).

Output frequency grids are taken from the second spectrum.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void sensor_responseFrequencySwitching(Workspace& ws) {
  sensor_responseFrequencySwitching(
      Var::sensor_response(ws), Var::sensor_response_f(ws),
      Var::sensor_response_pol(ws), Var::sensor_response_dlos(ws),
      Var::sensor_response_f_grid(ws), Var::sensor_response_pol_grid(ws),
      Var::sensor_response_dlos_grid(ws), Var::verbosity(ws));
}

/*! Simplified sensor setup for an AMSU-type instrument.

This function is derived from 'sensor_responseSimpleAMSU'
but is more generalized since the number of passbands in each
can be in the range from 1 to 4 - in order to correctly simulate
AMSU-A type sensors

This method allows quick and simple definition of AMSU-type
sensors. Assumptions:

1. Pencil beam antenna.
2. 1-4 Passband/sidebands per channel.
3. Sideband mode "upper"
4. The channel response is rectangular.

Under these assumptions the only inputs needed are the LO positions,
the offsets from the LO, and the IF bandwidths. They are provided
in sensor_description_amsu.

@author Oscar Isoz

@param[in,out] Workspace ws - An ARTS workspace
@param[in] spacing - Desired grid spacing in Hz. (default: .1e9)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void sensor_responseGenericAMSU(Workspace& ws, const Numeric& spacing = .1e9) {
  sensor_responseGenericAMSU(
      Var::f_grid(ws), Var::antenna_dim(ws), Var::mblock_dlos_grid(ws),
      Var::sensor_response(ws), Var::sensor_response_f(ws),
      Var::sensor_response_pol(ws), Var::sensor_response_dlos(ws),
      Var::sensor_response_f_grid(ws), Var::sensor_response_pol_grid(ws),
      Var::sensor_response_dlos_grid(ws), Var::sensor_norm(ws),
      Var::atmosphere_dim(ws), Var::stokes_dim(ws),
      Var::sensor_description_amsu(ws), spacing, Var::verbosity(ws));
}

/*! Converts sensor response variables from IF to RF.

The function converts intermediate frequencies (IF) in
*sensor_response_f* and *sensor_response_f_grid* to radio
frequencies (RF). This conversion is needed if the frequency
translation of a mixer is included and the position of backend
channels are specified in RF.

A direct frequency conversion is performed. Values are not
sorted in any way.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void sensor_responseIF2RF(Workspace& ws) {
  sensor_responseIF2RF(Var::sensor_response_f(ws),
                       Var::sensor_response_f_grid(ws), Var::lo(ws),
                       Var::sideband_mode(ws), Var::verbosity(ws));
}

/*! Initialises the variables summarising the sensor response.

This method sets the variables to match monochromatic pencil beam
calculations, to be further modified by inclusion of sensor
characteristics. Use *sensorOff* if pure monochromatic pencil
beam calculations shall be performed.

The variables are set as follows:
   sensor_response : Identity matrix, with size matching *f_grid*,
                     *stokes_dim* and *mblock_dlos_grid*.
   sensor_response_f       : Repeated values of *f_grid*.
   sensor_response_pol     : Data matching *stokes_dim*.
   sensor_response_dlos    : Repeated values of *mblock_dlos_grid*.
   sensor_response_f_grid  : Equal to *f_grid*.
   sensor_response_pol_grid: Set to 1:*stokes_dim*.
   sensor_response_dlos_grid : Equal to *mblock_dlos_grid*.

@author Mattias Ekstrom
@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void sensor_responseInit(Workspace& ws) {
  sensor_responseInit(
      Var::sensor_response(ws), Var::sensor_response_f(ws),
      Var::sensor_response_pol(ws), Var::sensor_response_dlos(ws),
      Var::sensor_response_f_grid(ws), Var::sensor_response_pol_grid(ws),
      Var::sensor_response_dlos_grid(ws), Var::f_grid(ws),
      Var::mblock_dlos_grid(ws), Var::antenna_dim(ws), Var::atmosphere_dim(ws),
      Var::stokes_dim(ws), Var::sensor_norm(ws), Var::verbosity(ws));
}

/*! Sensor setup for meteorological millimeter instruments.

This method is handy if you are simulating a passband-type instrument,
consisting of a few discrete channels.

For flexibility, the Met-MM system is seperated in two calculation
steps. To fully use the system, create *f_grid* (and some associated
variables) by *f_gridMetMM* before calling this method. However, it is
possible to use this method with any *f_grid*, as long as matching
*f_backend*, *channel2fgrid_indexes* and *channel2fgrid_weights*
are provided.

Each scan sequence is treated as a measurement block. *sensor_pos* is
set in the standard way. The number of rows in *sensor_pos* determines the
number of scan sequences that will be simulated. On the other hand,
*sensor_los* is handled in a special way. All zenith angles must be set
to 180 deg. For 3D, the given azimuth angles are taken as the direction
of scanning, where the azimuth angle is defined with respect to North
in standard manner. For example, if the scanning happens to move from
SW to NE, the azimuth angle should be set to 45 deg. The angles of the
scanning sequence are taken from *antenna_dlos*. This WSV is here only
allowed to have a single column, holding relative zenith angles. For
3D, the azimuth angles in *antenna_dlos* are hard-coded to zero. As
zenith angles in *sensor_los* are locked to 180 deg, *antenna_dlos*
effectively holds the nadir angles. These angles can be both positive or
negative, where the recommended choice is to operate with negative
to end up with final zenith angles between 0 and 180 deg.

The method does not support 2D atmospheres (across-track scanning is
inconsistent with 2D). For simpler switching between 1D and 3D,
the argument *mirror_dza* is at hand. It can only be used for 3D.
If set to true, the zenith angles in *antenna_dlos* are mapped
to also cover the other side of the swath and the simulations will
cover both sides of the swath.

@author Oliver Lemke
@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] use_antenna - Flag to enable (1) or disable (0) antenna. (default: 0)
@param[in] mirror_dza - Flag to include second part of swath (only 3D, see
above). (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void sensor_responseMetMM(Workspace& ws, const Index& use_antenna = 0,
                          const Index& mirror_dza = 0) {
  sensor_responseMetMM(
      Var::antenna_dim(ws), Var::mblock_dlos_grid(ws), Var::sensor_response(ws),
      Var::sensor_response_f(ws), Var::sensor_response_pol(ws),
      Var::sensor_response_dlos(ws), Var::sensor_response_f_grid(ws),
      Var::sensor_response_pol_grid(ws), Var::sensor_response_dlos_grid(ws),
      Var::sensor_norm(ws), Var::atmosphere_dim(ws), Var::stokes_dim(ws),
      Var::f_grid(ws), Var::f_backend(ws), Var::channel2fgrid_indexes(ws),
      Var::channel2fgrid_weights(ws), Var::iy_unit(ws), Var::antenna_dlos(ws),
      Var::met_mm_polarisation(ws), Var::met_mm_antenna(ws), use_antenna,
      mirror_dza, Var::verbosity(ws));
}

/*! Includes response of the mixer of a heterodyne system.

The function returns the sensor response matrix after the mixer
characteristics have been included. Frequency variables are
converted from radio frequency (RF) to intermediate frequency (IF).
The returned frequency grid covers the range [0,max_if], where
max_if is the highest IF covered by the sideband response grid.

See *lo* and *sideband_response* for details on how to specify the
mixer response

@author Mattias Ekstrom
@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void sensor_responseMixer(Workspace& ws) {
  sensor_responseMixer(
      Var::sensor_response(ws), Var::sensor_response_f(ws),
      Var::sensor_response_pol(ws), Var::sensor_response_dlos(ws),
      Var::sensor_response_f_grid(ws), Var::sensor_response_pol_grid(ws),
      Var::sensor_response_dlos_grid(ws), Var::lo(ws),
      Var::sideband_response(ws), Var::sensor_norm(ws), Var::verbosity(ws));
}

/*! Includes pre-calculated response covering mixer and backend.

This method acts similar to *sensor_responseBackend*, but uses
pre-calculated weights. These weights can also include the effect
of mixer and sideband filtering.

As usual, *f_backend* gives the frequency of the channels. This WSM
has no direct influence on the result, but at least representative
values must be set.

The frequency response is defined using *channel2fgrid_indexes* and
*channel2fgrid_weights*.

Both *channel2fgrid_indexes* and *channel2fgrid_weights* are assumed
to be common for all viewing directions.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void sensor_responseMixerBackendPrecalcWeights(Workspace& ws) {
  sensor_responseMixerBackendPrecalcWeights(
      Var::sensor_response(ws), Var::sensor_response_f(ws),
      Var::sensor_response_pol(ws), Var::sensor_response_dlos(ws),
      Var::sensor_response_f_grid(ws), Var::sensor_response_pol_grid(ws),
      Var::sensor_response_dlos_grid(ws), Var::f_backend(ws),
      Var::channel2fgrid_indexes(ws), Var::channel2fgrid_weights(ws),
      Var::verbosity(ws));
}

/*! Handles mixer and backend parts for an instrument having multiple
mixer chains.

The WSMs *sensor_responseMixer*, *sensor_responseIF2RF* and
*sensor_responseBackend* are called for each mixer chain, and a
complete *sensor_response* is assembled. The instrument responses
are described by *lo_multi*, *sideband_response_multi*,
*sideband_mode_multi*, *f_backend_multi* and
*backend_channel_response_multi*. All these WSVs must have same
vector or array length. As *sensor_responseIF2RF* is called,
*f_backend_multi* must hold RF (not IF) and output frequencies
will be in absolute frequency (RF).

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void sensor_responseMultiMixerBackend(Workspace& ws) {
  sensor_responseMultiMixerBackend(
      Var::sensor_response(ws), Var::sensor_response_f(ws),
      Var::sensor_response_pol(ws), Var::sensor_response_dlos(ws),
      Var::sensor_response_f_grid(ws), Var::sensor_response_pol_grid(ws),
      Var::sensor_response_dlos_grid(ws), Var::lo_multi(ws),
      Var::sideband_response_multi(ws), Var::sideband_mode_multi(ws),
      Var::f_backend_multi(ws), Var::backend_channel_response_multi(ws),
      Var::sensor_norm(ws), Var::verbosity(ws));
}

/*! Extraction of non-default polarisation components.

The default is to output the Stokes elements I, Q, U and V (up to
*stokes_dim*). This method allows to change the "polarisation" of
the output. Polarisation components to be extracted are selected by
*instrument_pol*. This method can be applied at any step of the sensor
matrix set-up.

The method can only be applied on data for I, Q, U and V. The value
of *stokes_dim* must be sufficiently large for the selected
components. For example, I+45 requires that *stokes_dim* is at
least 3.

See *instrument_pol* for coding of polarisation states.

Note that the state of *iy_unit* is considered. This WSV must give
the actual unit of the data. This as, the extraction of components
is slightly different if data are radiances or brightness
temperatures.  In practise this means that *iy_unit* (as to be
applied inside *iy_main_agenda*) must be set before calling this
method.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void sensor_responsePolarisation(Workspace& ws) {
  sensor_responsePolarisation(
      Var::sensor_response(ws), Var::sensor_response_f(ws),
      Var::sensor_response_pol(ws), Var::sensor_response_dlos(ws),
      Var::sensor_response_pol_grid(ws), Var::sensor_response_f_grid(ws),
      Var::sensor_response_dlos_grid(ws), Var::stokes_dim(ws), Var::iy_unit(ws),
      Var::instrument_pol(ws), Var::verbosity(ws));
}

/*! Simplified sensor setup for an AMSU-type instrument.

This method allows quick and simple definition of AMSU-type
sensors. Assumptions:

1. Pencil beam antenna.
2. Double sideband receivers.
3. Sideband mode "upper"
4. The channel response is rectangular.

Under these assumptions the only inputs needed are the LO positions,
the offsets from the LO, and the IF bandwidths. They are provieded
in sensor_description_amsu.

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace
@param[in] spacing - Desired grid spacing in Hz. (default: .1e9)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void sensor_responseSimpleAMSU(Workspace& ws, const Numeric& spacing = .1e9) {
  sensor_responseSimpleAMSU(
      Var::f_grid(ws), Var::antenna_dim(ws), Var::mblock_dlos_grid(ws),
      Var::sensor_response(ws), Var::sensor_response_f(ws),
      Var::sensor_response_pol(ws), Var::sensor_response_dlos(ws),
      Var::sensor_response_f_grid(ws), Var::sensor_response_pol_grid(ws),
      Var::sensor_response_dlos_grid(ws), Var::sensor_norm(ws),
      Var::atmosphere_dim(ws), Var::stokes_dim(ws),
      Var::sensor_description_amsu(ws), spacing, Var::verbosity(ws));
}

/*! Includes a rotation of the Stokes H and V directions.

The method applies the rotations implied by *stokes_rotation*.
See the description of that WSV for details.

This method does not change the size of *sensor_response*, and
the auxiliary variables (sensor_response_f etc.) are not changed.

To apply the method, *stokes_dim* must be >= 3. The complete effect
of the rotation can not be determibed with lower *stokes_dim*.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void sensor_responseStokesRotation(Workspace& ws) {
  sensor_responseStokesRotation(
      Var::sensor_response(ws), Var::sensor_response_f_grid(ws),
      Var::sensor_response_pol_grid(ws), Var::sensor_response_dlos_grid(ws),
      Var::stokes_dim(ws), Var::stokes_rotation(ws), Var::verbosity(ws));
}

/*! Adds WMRF weights to sensor response.

This method adds a spectrometer response that has been calculated
with the weighted mean of representative frequencies (WMRF) method. It
consists of a set of selected frequencies, and associated weights.

@author Stefan Buehler, based on Patrick Erikssons sensor_responseBackend

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void sensor_responseWMRF(Workspace& ws) {
  sensor_responseWMRF(
      Var::sensor_response(ws), Var::sensor_response_f(ws),
      Var::sensor_response_pol(ws), Var::sensor_response_dlos(ws),
      Var::sensor_response_f_grid(ws), Var::sensor_response_pol_grid(ws),
      Var::sensor_response_dlos_grid(ws), Var::wmrf_weights(ws),
      Var::f_backend(ws), Var::verbosity(ws));
}

/*! Calculates the spectral irradiance from *spectral_radiance_field* .
by integrating over the angular grids according to the grids set
by *AngularGridsSetFluxCalc*
See *AngularGridsSetFluxCalc to set
*za_grid, aa_grid, and za_grid_weights*

@author Manfred Brath

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void spectral_irradiance_fieldFromSpectralRadianceField(Workspace& ws) {
  spectral_irradiance_fieldFromSpectralRadianceField(
      Var::spectral_irradiance_field(ws), Var::spectral_radiance_field(ws),
      Var::za_grid(ws), Var::aa_grid(ws), Var::za_grid_weights(ws),
      Var::verbosity(ws));
}

/*! Clear-sky radiance field of a plane parallel atmosphere.

The method assumes a 1D flat planet. Radiances along each direction
given by *za_grid* are calculated using *ppathPlaneParallel*
and *iyEmissionStandard*.

Surface properties are defined by *iy_surface_agenda*, i.e. there is no
restriction to e.g. specular surfaces.

Note that the variable *ppath_lmax* is considered, and that it can be
critical for the accuracy for zenith angles close to 90 degrees. That
is, using ppath_lmax=-1 is not recommended for this function.

Information on transmission is also provided by the GOUT *trans_field*.
For up-welling radiation (scat_za > 90), this variable holds the
transmission to space, for considered position and propagation direction.
For down-welling radiation, *trans_field* holds instead the transmission
down to the surface.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] trans_field - Dimensions: [f_grid,p_grid,za_grid]. See further
above.
@param[in] use_parallel_iy - 0: Parallelize over zenith angles
1: Use more memory intensiv iyEmissionStandardParallel* (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void spectral_radiance_fieldClearskyPlaneParallel(
    Workspace& ws, Tensor3& trans_field, const Index& use_parallel_iy = 0) {
  spectral_radiance_fieldClearskyPlaneParallel(
      ws, Var::spectral_radiance_field(ws), trans_field,
      Var::propmat_clearsky_agenda(ws), Var::water_p_eq_agenda(ws),
      Var::iy_space_agenda(ws), Var::iy_surface_agenda(ws),
      Var::iy_cloudbox_agenda(ws), Var::stokes_dim(ws), Var::f_grid(ws),
      Var::atmosphere_dim(ws), Var::p_grid(ws), Var::z_field(ws),
      Var::t_field(ws), Var::nlte_field(ws), Var::vmr_field(ws),
      Var::abs_species(ws), Var::wind_u_field(ws), Var::wind_v_field(ws),
      Var::wind_w_field(ws), Var::mag_u_field(ws), Var::mag_v_field(ws),
      Var::mag_w_field(ws), Var::z_surface(ws), Var::ppath_lmax(ws),
      Var::rte_alonglos_v(ws), Var::surface_props_data(ws), Var::za_grid(ws),
      use_parallel_iy, Var::verbosity(ws));
}

/*! Set *spectral_radiance_field* to be a copy of *cloudbox_field*.

This method can only be used for 1D atmospheres and if the cloud
box covers the complete atmosphere. For such case, the two fields
cover the same atmospheric volume and a direct copying can be made.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void spectral_radiance_fieldCopyCloudboxField(Workspace& ws) {
  spectral_radiance_fieldCopyCloudboxField(
      Var::spectral_radiance_field(ws), Var::atmosphere_dim(ws),
      Var::p_grid(ws), Var::cloudbox_on(ws), Var::cloudbox_limits(ws),
      Var::cloudbox_field(ws), Var::verbosity(ws));
}

/*! Uses and expands *cloudbox_field* to set *spectral_radiance_field*.

The method demands that *cloudbox_field* starts at the first pressure
level (i.e. cloudbox_limits[0] is 0). The method copies *cloudbox_field*
to fill *spectral_radiance_field* up to the top of the cloudbox.

To fill the remaning part of *spectral_radiance_field*, clear-sky
calculations are performed largely in the same maner as done by
*spectral_radiance_fieldClearskyPlaneParallel*. That is, clear-sky
calculations are done for the upper part of the atmosphere, assuming
a flat planet.

Note that the cloud box constitutes the lower boundary for the later
calculations, and *iy_cloudbox_agenda* must be set to perform an
interpolation of the cloudbox field.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] use_parallel_iy - 0: Parallelize over zenith angles
1: Use more memory intensiv iyEmissionStandardParallel* (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void spectral_radiance_fieldExpandCloudboxField(
    Workspace& ws, const Index& use_parallel_iy = 0) {
  spectral_radiance_fieldExpandCloudboxField(
      ws, Var::spectral_radiance_field(ws), Var::propmat_clearsky_agenda(ws),
      Var::water_p_eq_agenda(ws), Var::iy_space_agenda(ws),
      Var::iy_surface_agenda(ws), Var::iy_cloudbox_agenda(ws),
      Var::stokes_dim(ws), Var::f_grid(ws), Var::atmosphere_dim(ws),
      Var::p_grid(ws), Var::z_field(ws), Var::t_field(ws), Var::nlte_field(ws),
      Var::vmr_field(ws), Var::abs_species(ws), Var::wind_u_field(ws),
      Var::wind_v_field(ws), Var::wind_w_field(ws), Var::mag_u_field(ws),
      Var::mag_v_field(ws), Var::mag_w_field(ws), Var::z_surface(ws),
      Var::cloudbox_on(ws), Var::cloudbox_limits(ws), Var::cloudbox_field(ws),
      Var::ppath_lmax(ws), Var::rte_alonglos_v(ws), Var::surface_props_data(ws),
      Var::za_grid(ws), use_parallel_iy, Var::verbosity(ws));
}

/*! Calculates the specular direction of surface reflections.

A help method to set up the surface properties. This method
calculates *specular_los*, that is required in several methods
to convert zenith angles to incidence angles.

The method also returns the line-of-sight matching the surface normal.

The default is to consider the surface slope when calculating the
specular direction. That is, the variation of *z_surface* (as well as
the geoid radius) is considered and the specular direction is calculated
including the specified topography. This part can be deactivated by
setting *ignore_surface_slope* to 1. In this case, the zenith angle of
the specular direction is simply 180-rtp_los[0]. *ignore_surface_slope*
has only an effect for 2D and 3D, as 1D implies a constant radius of
the surface (i.e. no topography).

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] ignore_surface_slope - Flag to control if surface slope is consdered
or not. (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void specular_losCalc(Workspace& ws, const Index& ignore_surface_slope = 0) {
  specular_losCalc(Var::specular_los(ws), Var::surface_normal(ws),
                   Var::rtp_pos(ws), Var::rtp_los(ws), Var::atmosphere_dim(ws),
                   Var::lat_grid(ws), Var::lon_grid(ws), Var::refellipsoid(ws),
                   Var::z_surface(ws), ignore_surface_slope,
                   Var::verbosity(ws));
}

/*! Calculates the specular direction of surface reflections for horisontal
surfaces.

In contrast to *specular_losCalc*, this method ignores the topography
implied by *z_surface*. That is, any slope of the surface is ignored.

The typical application of this WSM should be water surfaces (lakes and
oceans).

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void specular_losCalcNoTopography(Workspace& ws) {
  specular_losCalcNoTopography(Var::specular_los(ws), Var::surface_normal(ws),
                               Var::rtp_pos(ws), Var::rtp_los(ws),
                               Var::atmosphere_dim(ws), Var::verbosity(ws));
}

/*! Creates variables to mimic a blackbody surface.

This method sets up *surface_los*, *surface_rmatrix* and
*surface_emission* for *surface_rtprop_agenda*. Here, *surface_los*
and *surface_rmatrix* are set to be empty, and *surface_emission*
to hold blackbody radiation for a temperature of *surface_skin_t*.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void surfaceBlackbody(Workspace& ws) {
  surfaceBlackbody(Var::surface_los(ws), Var::surface_rmatrix(ws),
                   Var::surface_emission(ws), Var::atmosphere_dim(ws),
                   Var::f_grid(ws), Var::stokes_dim(ws), Var::rtp_pos(ws),
                   Var::rtp_los(ws), Var::surface_skin_t(ws),
                   Var::verbosity(ws));
}

/*! Usage of FASTEM together with MC and DOIT.

The recommended way to use FASTEM is by *iySurfaceFastem*, but that
is not always possible, such as when using MC and DOIT. This is the
case as those scattering methods use *surface_rtprop_agenda*,
while *iySurfaceFastem* fits with *iy_surface_agenda*. This WSM solves
this by allowing FASTEM to be used inside *surface_rtprop_agenda*.

However, FASTEM is here used in an approximative way. For a correct
usage of FASTEM, the atmospheric transmittance shall be calculated
for the position and direction of concern, but this is not possible
together with DOIT and MC. Instead, the transmittance is an input
to the method, and must either be pre-calculated or set to a
representative value.

See *iySurfaceFastem*, for further details on the special input
arguments.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] salinity - Salinity, 0-1. That is, 3% is given as 0.03. (default:
0.035)
@param[in] wind_speed - Wind speed.
@param[in] wind_direction - Wind direction. See futher above. (default: 0)
@param[in] transmittance - Transmittance along path of downwelling radiation. A
vector with the same length as *f_grid*.
@param[in] fastem_version - The version of FASTEM to use. (default: 6)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void surfaceFastem(Workspace& ws, const Numeric& wind_speed,
                   const Vector& transmittance, const Numeric& salinity = 0.035,
                   const Numeric& wind_direction = 0,
                   const Index& fastem_version = 6) {
  surfaceFastem(Var::surface_los(ws), Var::surface_rmatrix(ws),
                Var::surface_emission(ws), Var::atmosphere_dim(ws),
                Var::stokes_dim(ws), Var::f_grid(ws), Var::rtp_pos(ws),
                Var::rtp_los(ws), Var::surface_skin_t(ws), salinity, wind_speed,
                wind_direction, transmittance, fastem_version,
                Var::verbosity(ws));
}

/*! Creates variables to mimic specular reflection by a (flat) surface
where *surface_reflectivity* is specified.

Works basically as *surfaceFlatScalarReflectivity* but is more
general as vector radiative transfer is more properly handled. See
the ARTS theory document (ATD) for details around how
*surface_emission* is determined. In the nomenclature of ATD,
*surface_reflectivity* gives R.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void surfaceFlatReflectivity(Workspace& ws) {
  surfaceFlatReflectivity(Var::surface_los(ws), Var::surface_rmatrix(ws),
                          Var::surface_emission(ws), Var::f_grid(ws),
                          Var::stokes_dim(ws), Var::atmosphere_dim(ws),
                          Var::rtp_pos(ws), Var::rtp_los(ws),
                          Var::specular_los(ws), Var::surface_skin_t(ws),
                          Var::surface_reflectivity(ws), Var::verbosity(ws));
}

/*! Creates variables to mimic specular reflection by a (flat) surface
where the complex refractive index is specified.

The dielectric properties of the surface are described by
*surface_complex_refr_index*. The Fresnel equations are used to
calculate amplitude reflection coefficients. The method can thus
result in that the reflection properties differ between frequencies
and polarisations.

Local thermodynamic equilibrium is assumed, which corresponds to
that the reflection and emission coefficients add up to 1.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void surfaceFlatRefractiveIndex(Workspace& ws) {
  surfaceFlatRefractiveIndex(
      Var::surface_los(ws), Var::surface_rmatrix(ws), Var::surface_emission(ws),
      Var::f_grid(ws), Var::stokes_dim(ws), Var::atmosphere_dim(ws),
      Var::rtp_pos(ws), Var::rtp_los(ws), Var::specular_los(ws),
      Var::surface_skin_t(ws), Var::surface_complex_refr_index(ws),
      Var::verbosity(ws));
}

/*! Creates variables to mimic specular reflection by a (flat) surface
where *surface_rv_rh* is specified.

This method assumes that the reflection at vertical and horizontal
polarisation differs. As power reflection coefficients are provided
there is no information at hand on phase shifts between polarisations,
and they are simply assumed to be zero. These assumptions result in
that *surface_emission* is set to zero for positions corresponding to
U and V, and that all diagonal elementsof  *surface_rmatrix* are equal
(the mean of rv and rh). Further, all off-diagonal elements of
*surface_rmatrix* are all zero except for (0,1) and (1,0).

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void surfaceFlatRvRh(Workspace& ws) {
  surfaceFlatRvRh(
      Var::surface_los(ws), Var::surface_rmatrix(ws), Var::surface_emission(ws),
      Var::f_grid(ws), Var::stokes_dim(ws), Var::atmosphere_dim(ws),
      Var::rtp_pos(ws), Var::rtp_los(ws), Var::specular_los(ws),
      Var::surface_skin_t(ws), Var::surface_rv_rh(ws), Var::verbosity(ws));
}

/*! Creates variables to mimic specular reflection by a (flat) surface
where *surface_scalar_reflectivity* is specified.

This method assumes that the reflection at vertical and horizontal
polarisation is identical. This assumption includes that there is no
phase shift between polarisations. These assumptions result in that
*surface_emission* is set to zero for positions corresponding to Q,
U and V, and that *surface_rmatrix* becomes a diagonal matrix (with
all elements on the diagonal equal to the specified reflectivity).

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void surfaceFlatScalarReflectivity(Workspace& ws) {
  surfaceFlatScalarReflectivity(
      Var::surface_los(ws), Var::surface_rmatrix(ws), Var::surface_emission(ws),
      Var::f_grid(ws), Var::stokes_dim(ws), Var::atmosphere_dim(ws),
      Var::rtp_pos(ws), Var::rtp_los(ws), Var::specular_los(ws),
      Var::surface_skin_t(ws), Var::surface_scalar_reflectivity(ws),
      Var::verbosity(ws));
}

/*! Creates variables to mimic a Lambertian surface.

A Lambertian surface can be characterised solely by its
reflectivity, here taken from *surface_scalar_reflectivity*.

The down-welling radiation field is estimated by making calculations
for *lambertian_nza* directions. The range of zenith angles ([0,90])
is divided in an equidistant manner for 1D. For 2D and 3D see below.
The values for *surface_rmatrix* are assuming a constant radiance
over each zenith angle range. See AUG.

Default is to select the zenith angles for *sensor_los* to be placed
centrally in the grid ranges. For example, if *lambertian_nza* is set
to 9, down-welling radiation will be calculated for zenith angles =
5, 15, ..., 85. The position of these angles can be shifted by
*za_pos*. This variable specifies the fractional distance inside the
ranges. For example, a *za_pos* of 0.7 (np still 9) gives the angles
7, 17, ..., 87.

Only upper-left diagonal element of the *surface_rmatrix* is
non-zero. That is, the upwelling radiation is always unpolarised.

Local thermodynamic equilibrium is assumed, which corresponds to
that the reflection and emission coefficients "add up to 1".

For 2D and 3D, the down-welling directions are placed along the
the viewing direction, e.g. for 3D the azimuth angle is kept constant.
In 2D and 3D surface topography can exist, and to avoid getting views
going directly into the surface, angels are not distributed over 90 deg,
but 90-abs(surface_normal[0]).

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] lambertian_nza - Number of downwelling streams. (default: 9)
@param[in] za_pos - Position of angle in *surface_los* inside ranges of zenith
angle grid. See above. (default: 0.5)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void surfaceLambertianSimple(Workspace& ws, const Index& lambertian_nza = 9,
                             const Numeric& za_pos = 0.5) {
  surfaceLambertianSimple(
      Var::surface_los(ws), Var::surface_rmatrix(ws), Var::surface_emission(ws),
      Var::f_grid(ws), Var::stokes_dim(ws), Var::atmosphere_dim(ws),
      Var::rtp_pos(ws), Var::rtp_los(ws), Var::surface_normal(ws),
      Var::surface_skin_t(ws), Var::surface_scalar_reflectivity(ws),
      lambertian_nza, za_pos, Var::verbosity(ws));
}

/*! A simplistic treatment of semi-specular surfaces.

This method has no strong physical basis but could be used for simpler
testing or as starting point for more advanced methods.

This method assumes that the surface can be treated to have three facets,
all lacking surface roughness. One facet is assumed to give standard
specular reflection, while the two other facets are tilted with +dza
and -dza, respectively. The tilt is assumed to only affect the zenith
angle of the reflected direction (azimuth same as for specular direction).
The area ratio of the non-tilted facet is set by *specular_factor*.
That is, the specular beam is given weight w, while the other two beams
each get weight (1-w)/2.

If a facet tilts away from the viewing direction in such way that
the surface is observed from below, the tilt of the facet is decreased
in steps of 1 degree until a successful calculation is obtained. If this
turns out to require a tilt of zero, this facete is merged with
the specular direction.

The pure specular properties of the surface shall be described by
*surface_rtprop_sub_agenda*. That is, if you have specular surface
described and you want to make it semi-specular by this method, you
move the content of the existing *surface_rtprop_agenda* to
*surface_rtprop_sub_agenda* and instead fill *surface_rtprop_agenda*
with this method.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] specular_factor - The weight given to the specular direction. Denoted
as w above. A value between 1/3 and 1.
@param[in] dza - Zenith angle seperation to each secondary direction. A between
0 and 45 degrees.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void surfaceSemiSpecularBy3beams(Workspace& ws, const Numeric& specular_factor,
                                 const Numeric& dza) {
  surfaceSemiSpecularBy3beams(
      ws, Var::surface_skin_t(ws), Var::surface_los(ws),
      Var::surface_rmatrix(ws), Var::surface_emission(ws),
      Var::atmosphere_dim(ws), Var::f_grid(ws), Var::rtp_pos(ws),
      Var::rtp_los(ws), Var::surface_rtprop_sub_agenda(ws), specular_factor,
      dza, Var::verbosity(ws));
}

/*! A very simple approximation of a semi-specular surface.

This method has no direct physical basis but could be used for simpler
testing or as starting point for more advanced methods.

The method requires that the surface RT properties (e.g. *surface_los*)
have been set up to mimic a specular surface. This method splits the down-
welling radiation into three directions. The specular direction is given
weight w, while the other two beams each get weight (1-w)/2. The basic
polarised reflectivity from the specular calculations is maintained
for each beam. The beams are just separated in zenith angle, with a
separation of *dza*. The lowermost beam is not allowed to be closer to
the surface than 1 degree. If there is no room for the lowermost beam,
it is merged with the main beam.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] specular_factor - The weight given to the specular direction. Denoted
as w above. A value between 1/3 and 1.
@param[in] dza - Zenith angle seperation to each secondary direction. A between
0 and 45 degrees.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void surfaceSplitSpecularTo3beams(Workspace& ws, const Numeric& specular_factor,
                                  const Numeric& dza) {
  surfaceSplitSpecularTo3beams(Var::surface_los(ws), Var::surface_rmatrix(ws),
                               Var::atmosphere_dim(ws), Var::rtp_pos(ws),
                               Var::rtp_los(ws), specular_factor, dza,
                               Var::verbosity(ws));
}

/*! Compute surface emissivities using the TELSEM 2 model.

This method uses second version of the TELSEM model for calculating
land surface emissivities (F. Aires et al, "A Tool to Estimate
 Land‐Surface Emissivities at Microwave frequencies (TELSEM) for use
 in numerical weather prediction" Quarterly Journal of the Royal
Meteorological Society, vol. 137, (656), pp. 690-699, 2011.)
This methods computes land surface emissivities for a given pencil beam
using a given TELSEM2 atlas.
The input must satisfy the following conditions, otherwise an error is thrown:
 - The input frequencies (*f_grid*) must be within the range [5 GHz, 900 GHz]
 - The skin temperature (*surface_skin_t*) must be within the range
   [180 K, 360 K]

A TELSEM atlas contains only suface emissivities for locations that are
classified as land. By default this WSM will throw an error if the
pencil beam hits the surface at a position that is not contained in the
given atlas.

The above behavior can be avoided by setting *d_max* to a positive value.
This enables nearest neighbor interpolation, which assigns the emissivities
of the nearest found cell in the atlas to the given position. In this case,
an error is only thrown if the distance of the found neighbor is higher
than the provided value of *d_max.

You can limit the final reflectivity applied by setting *r_min* and *r_max*.

To extract a land-sea mask from a given telsem atlas see the WSM
*telsemSurfaceTypeLandSea*.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[in] atlas - The Telsem atlas to use for the emissivity calculation.
@param[in] r_min - Minimum allowed value for reflectivity to apply. (default: 0)
@param[in] r_max - Maximum allowed value for reflectivity to apply. (default: 1)
@param[in] d_max - Maximum allowed distance in meters for nearest neighbor
interpolation in meters. Set to a negative value or zero  to disable
interpolation. (default: -1.0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void surfaceTelsem(Workspace& ws, const TelsemAtlas& atlas,
                   const Numeric& r_min = 0, const Numeric& r_max = 1,
                   const Numeric& d_max = -1.0) {
  surfaceTelsem(Var::surface_los(ws), Var::surface_rmatrix(ws),
                Var::surface_emission(ws), Var::atmosphere_dim(ws),
                Var::stokes_dim(ws), Var::f_grid(ws), Var::lat_grid(ws),
                Var::lat_true(ws), Var::lon_true(ws), Var::rtp_pos(ws),
                Var::rtp_los(ws), Var::surface_skin_t(ws), atlas, r_min, r_max,
                d_max, Var::verbosity(ws));
}

/*! TESSEM sea surface microwave emissivity parametrization.

This method computes surface emissivity and reflectivity matrices for
ocean surfaces using the TESSEM emissivity model: Prigent, C., et al.
Sea‐surface emissivity parametrization from microwaves to millimetre
waves, QJRMS, 2017, 143.702: 596-605.

The validity range of the parametrization of is 10 to 700 GHz, but for
some extra flexibility frequencies between 5 and 900 GHz are accepted.
The accepted temperaute range for *surface_skin_t* is [260.0 K, 373.0 K]

The model itself is represented by the neural networks in
*tessem_neth* and *tessem_netv*.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[in] salinity - Salinity, 0-1. That is, 3% is given as 0.03. (default:
0.035)
@param[in] wind_speed - Wind speed.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void surfaceTessem(Workspace& ws, const Numeric& wind_speed,
                   const Numeric& salinity = 0.035) {
  surfaceTessem(Var::surface_los(ws), Var::surface_rmatrix(ws),
                Var::surface_emission(ws), Var::atmosphere_dim(ws),
                Var::stokes_dim(ws), Var::f_grid(ws), Var::rtp_pos(ws),
                Var::rtp_los(ws), Var::surface_skin_t(ws), Var::tessem_neth(ws),
                Var::tessem_netv(ws), salinity, wind_speed, Var::verbosity(ws));
}

/*! Extracts complex refractive index from a field of such data.

The method allows to obtain *surface_complex_refr_index* by
interpolation of a geographical field of such data. The position
for which refraction shall be extracted is given by *rtp_pos*.
The refractive index field is expected to be stored as:
   GriddedField5:
      Vector f_grid[N_f]
      Vector T_grid[N_T]
      ArrayOfString Complex[2]
      Vector "Latitude"  [N_lat]
      Vector "Longitude" [N_lon]
      Tensor5 data[N_f][N_T][2][N_lat][N_lon]

Definition and treatment of the three first dimensions follows
*complex_refr_index*, e.g. the temperature grid is allowed
to have length 1. The grids for latitude and longitude must have
a length of >= 2 (ie. no automatic expansion).

Hence, this method performs an interpolation only in the lat and
lon dimensions, to a single point. The remaining GriddedField3 is
simply returned as *surface_complex_refr_index*.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] complex_refr_index_field - A field of complex refractive index.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void surface_complex_refr_indexFromGriddedField5(
    Workspace& ws, const GriddedField5& complex_refr_index_field) {
  surface_complex_refr_indexFromGriddedField5(
      Var::surface_complex_refr_index(ws), Var::atmosphere_dim(ws),
      Var::lat_grid(ws), Var::lat_true(ws), Var::lon_true(ws), Var::rtp_pos(ws),
      complex_refr_index_field, Var::verbosity(ws));
}

/*! Extracts surface reflectivities from a field of such data.

This method allows to specify a field of surface reflectivity for
automatic interpolation to points of interest. The position and
direction for which the reflectivity shall be extracted are given
by *rtp_pos* and *rtp_los*. The reflectivity field is expected to
be stored as:
   GriddedField6:
      Vector "Frequency"       [N_f]
      Vector "Stokes element"  [N_s1]
      Vector "Stokes_element"  [N_s2]
      Vector "Incidence angle" [N_ia]
      Vector "Latitude"        [N_lat]
      Vector "Longitude"       [N_lon]
      Tensor6 data[N_f][N_s1][N_s2][N_ia][N_lat][N_lon]

Grids for incidence angle, latitude and longitude must have a
length of >= 2 (ie. no automatic expansion). If the frequency grid
has length 1, this is taken as that the reflectivity is constant,
following the definition of *surface_scalar_reflectivity*.
The data can cover higher Stokes dimensionalities than set by
*stokes_dim*. Data for non-used Stokes elements are just cropped.
The order between the two Stokes dimensions is the same as in
*surface_reflectivity* and surface_rmatrix*.

The interpolation is done in steps:
   1: Linear interpolation for lat and lon (std. extrapolation).
   2: Interpolation in incidence angle (std. extrapolation).
      If the grid has a length of >= 4, cubic interpolation is
      applied. Otherwise linear interpolation.
   3. Linear interpolation in frequency (if input data have more
      than one frequency).

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] r_field - A field of surface reflectivities

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void surface_reflectivityFromGriddedField6(Workspace& ws,
                                           const GriddedField6& r_field) {
  surface_reflectivityFromGriddedField6(
      Var::surface_reflectivity(ws), Var::stokes_dim(ws), Var::f_grid(ws),
      Var::atmosphere_dim(ws), Var::lat_grid(ws), Var::lat_true(ws),
      Var::lon_true(ws), Var::rtp_pos(ws), Var::rtp_los(ws), r_field,
      Var::verbosity(ws));
}

/*! Switch between the elements of *surface_rtprop_agenda_array*.

This method simply calls the agenda matching *surface_type* and
returns the results. That is, the agenda in *surface_rtprop_agenda_array*
with index *surface_type* (0-based) is called.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void surface_rtpropCallAgendaX(Workspace& ws) {
  surface_rtpropCallAgendaX(
      ws, Var::surface_skin_t(ws), Var::surface_los(ws),
      Var::surface_rmatrix(ws), Var::surface_emission(ws), Var::f_grid(ws),
      Var::rtp_pos(ws), Var::rtp_los(ws), Var::surface_rtprop_agenda_array(ws),
      Var::surface_type(ws), Var::surface_type_aux(ws), Var::verbosity(ws));
}

/*! Extracts scalar surface reflectivities from a field of such data.

This method allows to specify a field of surface reflectivity for
automatic interpolation to points of interest. The position and
direction for which the reflectivity shall be extracted are given
by *rtp_pos* and *rtp_los*. The reflectivity field is expected to
be stored as:
   GriddedField4:
      Vector "Frequency"       [N_f]
      Vector "Incidence angle" [N_ia]
      Vector "Latitude"        [N_lat]
      Vector "Longitude"       [N_lon]
      Tensor4 data[N_f][N_ia][N_lat][N_lon]

Grids for incidence angle, latitude and longitude must have a
length of >= 2 (ie. no automatic expansion). If the frequency grid
has length 1, this is taken as the reflectivity is constant,
following the definition of *surface_scalar_reflectivity*.

The interpolation is done in steps:
   1: Linear interpolation for lat and lon (std. extrapolation).
   2: Interpolation in incidence angle (std. extrapolation).
      If the grid has a length of >= 4, cubic interpolation is
      applied. Otherwise linear interpolation.
   3. Linear interpolation if frequency (if input data have more
      than one frequency).

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] r_field - A field of scalar surface reflectivities

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void surface_scalar_reflectivityFromGriddedField4(
    Workspace& ws, const GriddedField4& r_field) {
  surface_scalar_reflectivityFromGriddedField4(
      Var::surface_scalar_reflectivity(ws), Var::stokes_dim(ws),
      Var::f_grid(ws), Var::atmosphere_dim(ws), Var::lat_grid(ws),
      Var::lat_true(ws), Var::lon_true(ws), Var::rtp_pos(ws), Var::rtp_los(ws),
      r_field, Var::verbosity(ws));
}

/*! Sets *surface_scalar_reflectivity* based on *surface_rmatrix*.

For each frequency f, *surface_scalar_reflectivity* is set to
the sum of surface_rmatrix(joker,f,0,0).

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void surface_scalar_reflectivityFromSurface_rmatrix(Workspace& ws) {
  surface_scalar_reflectivityFromSurface_rmatrix(
      Var::surface_scalar_reflectivity(ws), Var::surface_rmatrix(ws),
      Var::verbosity(ws));
}

/*! Closest neighbour interpolation of surface type mask.

The method determines the surface type at the position of concern
(*rtp_pos*) from the provided type mask (*surface_type_mask*).
The closest point in the mask is selected. The surface type
is set to the integer part of the value at the found point, while
*surface_type_aux* is set to the reminder. For example, if the
mask value at closest point is 2.23, *surface_type* is set to 2
*surface_type_aux* becomes 0.23.

The altitude in *rtp_pos* is ignored.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void surface_typeInterpTypeMask(Workspace& ws) {
  surface_typeInterpTypeMask(
      Var::surface_type(ws), Var::surface_type_aux(ws), Var::atmosphere_dim(ws),
      Var::lat_grid(ws), Var::lat_true(ws), Var::lon_true(ws), Var::rtp_pos(ws),
      Var::surface_type_mask(ws), Var::verbosity(ws));
}

/*! Lookup SSMI emissivities from Telsem Atlas.

This returns the emissivities (indices [0,..,6])
 for the SSMI channels that are contained in
the Telsem atlas.

If given latitude and longitude are not in the atlas an empty
vector is returned.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[out] emissivities - The SSMI emissivities from the atlas
@param[in] lat - The latitude for which to compute the emissivities.
@param[in] lon - The latitude for which to compute the emissivities.
@param[in] atlas - The Telsem atlas to use.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void telsemAtlasLookup(Workspace& ws, Vector& emissivities, const Numeric& lat,
                       const Numeric& lon, const TelsemAtlas& atlas) {
  telsemAtlasLookup(emissivities, lat, lon, atlas, Var::verbosity(ws));
}

/*! Stand-alone evaluation of the Telsem model.

This evaluates the Telsem land surface emissivity
model using the data from the provided atlas.

Since TELSEM atlases do not contain data for all locations
this function allows for nearest neighbor interpolation, which
can be enabled by setting the *d_max* GIN to a positive value.

This WSM throws a runtime error if the queried location is not
contained in the atlas or the distance of the neighboring cell
exceeds the given *d_max* value.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[out] emissivities - The computed h and v emissivites
@param[in] lat - The latitude for which to compute the emissivities.
@param[in] lon - The latitude for which to compute the emissivities.
@param[in] theta - The incidence angle.
@param[in] f - The frequencies for which to compute the emissivities.
@param[in] ta - The Telsem atlas to use.
@param[in] d_max - The maximum allowed distance for nearest neighbor
interpolation in meters. (default: -1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void telsemStandalone(Workspace& ws, Matrix& emissivities, const Numeric& lat,
                      const Numeric& lon, const Numeric& theta, const Vector& f,
                      const TelsemAtlas& ta, const Numeric& d_max = -1) {
  telsemStandalone(emissivities, lat, lon, theta, f, ta, d_max,
                   Var::verbosity(ws));
}

/*! TELSEM based land sea mask.

This method determines whether the position in *rtp_pos* is
of type ocean or land depending on whether a corresponding
cell is contained in the provided TELSEM atlas.
In combination with the WSM *surface_rtpropCallAgendaX* this
can be used to used different methods to compute surface radiative
properties.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[in] atlas - The telsem atlas from which to lookup the surface type.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void telsemSurfaceTypeLandSea(Workspace& ws, const TelsemAtlas& atlas) {
  telsemSurfaceTypeLandSea(Var::surface_type(ws), Var::atmosphere_dim(ws),
                           Var::lat_grid(ws), Var::lat_true(ws),
                           Var::lon_true(ws), Var::rtp_pos(ws), atlas,
                           Var::verbosity(ws));
}

/*! Reads single TELSEM atlas.

'directory' needs to contain the original 12 Telsem atlas files
and the correlations file. This WSM reads the atlas for the specified
month and stores the result in the provided output atlas.

@author Simon Pfreundschuh

@param[in,out] Workspace ws - An ARTS workspace
@param[out] atlas - The atlas into which to store the loaded atlas.
@param[in] directory - Directory with TELSEM 2 SSMI atlas files.
@param[in] month - The month for which the atlas should be read.
@param[in] filename_pattern - Filename pattern (@MM@ gets replaced by month
number) (default: "ssmi_mean_emis_climato_@MM@_cov_interpol_M2")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void telsem_atlasReadAscii(Workspace& ws, TelsemAtlas& atlas,
                           const String& directory, const Index& month,
                           const String& filename_pattern =
                               "ssmi_mean_emis_climato_@MM@_cov_interpol_M2") {
  telsem_atlasReadAscii(atlas, directory, month, filename_pattern,
                        Var::verbosity(ws));
}

/*! Reads TELSEM atlas files.

'directory' needs to contain the original 12 Telsem atlas files
and the correlations file.
The whole data is combined into the WSV *telsem_atlases*

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] directory - Directory with TELSEM 2 SSMI atlas files.
@param[in] filename_pattern - Filename pattern (@MM@ gets replaced by month
number) (default: "ssmi_mean_emis_climato_@MM@_cov_interpol_M2")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void telsem_atlasesReadAscii(
    Workspace& ws, const String& directory,
    const String& filename_pattern =
        "ssmi_mean_emis_climato_@MM@_cov_interpol_M2") {
  telsem_atlasesReadAscii(Var::telsem_atlases(ws), directory, filename_pattern,
                          Var::verbosity(ws));
}

/*! Sets time to system_clock::now().

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void timeNow(Workspace& ws) { timeNow(Var::time(ws), Var::verbosity(ws)); }

/*! Offsets time for some seconds

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] offset - Time in seconds

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void timeOffset(Workspace& ws, const Numeric& offset) {
  timeOffset(Var::time(ws), offset, Var::verbosity(ws));
}

/*! Sleeps until time has been reached.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void timeSleep(Workspace& ws) { timeSleep(Var::time(ws), Var::verbosity(ws)); }

/*! Offsets a time grid by some seconds.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] dt - Time in seconds to add

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void time_gridOffset(Workspace& ws, const Numeric& dt) {
  time_gridOffset(Var::time_grid(ws), dt, Var::verbosity(ws));
}

/*! Sort *in* by *time_stamps* into *out*.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] out - Array sorted by time
@param[in] in - Array to sort of same size as *time_stamps*

Use the ARTS documentation to read more on how the workspace is manipulated
*/
template <typename Any_0, typename Any_1>
void time_stampsSort(Workspace& ws, Any_0& out, const Any_1& in) {
  time_stampsSort(out, Var::time_stamps(ws), in, Var::verbosity(ws));
}

/*! Initializes the CPU timer.
Use *timerStop* to stop the timer.

Usage example:
   timerStart
   ReadXML(f_grid,"frequencies.xml")
   timerStop
   Print(timer)

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void timerStart(Workspace& ws) {
  timerStart(Var::timer(ws), Var::verbosity(ws));
}

/*! Stops the CPU timer.
See *timerStart* for example usage.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void timerStop(Workspace& ws) { timerStop(Var::timer(ws), Var::verbosity(ws)); }

/*! Creates a vector of transmittance values.

The transmittances are set based on optical depths in *iy_aux*. That is,
one of the quantities in *iy_aux* must be "Optical depth".

The created vector has a length matching *f_grid* and can e.g. be used
as input to some of the FASTEM methods.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[out] transmittance - Created vector of transmittance values.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void transmittanceFromIy_aux(Workspace& ws, Vector& transmittance) {
  transmittanceFromIy_aux(transmittance, Var::iy_aux_vars(ws), Var::iy_aux(ws),
                          Var::verbosity(ws));
}

/*! Initializes the verbosity levels.

Sets verbosity to defaults or the levels specified by -r on the command line.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void verbosityInit(Workspace& ws) { verbosityInit(Var::verbosity(ws)); }

/*! Sets the verbosity levels.

Sets the reporting level for agenda calls, screen and file.
All reporting levels can reach from 0 (only error messages)
to 3 (everything). The agenda setting applies in addition
to both screen and file output.

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] agenda - Agenda verbosity level
@param[in] screen - Screen verbosity level
@param[in] file - Report file verbosity level

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void verbositySet(Workspace& ws, const Index& agenda, const Index& screen,
                  const Index& file) {
  verbositySet(Var::verbosity(ws), agenda, screen, file);
}

/*! Sets the verbosity level for agenda output.

See *verbositySet*

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] level - Agenda verbosity level

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void verbositySetAgenda(Workspace& ws, const Index& level) {
  verbositySetAgenda(Var::verbosity(ws), level);
}

/*! Sets the verbosity level for report file output.

See *verbositySet*

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] level - Report file verbosity level

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void verbositySetFile(Workspace& ws, const Index& level) {
  verbositySetFile(Var::verbosity(ws), level);
}

/*! Sets the verbosity level for screen output.

See *verbositySet*

@author Oliver Lemke

@param[in,out] Workspace ws - An ARTS workspace
@param[in] level - Screen verbosity level

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void verbositySetScreen(Workspace& ws, const Index& level) {
  verbositySetScreen(Var::verbosity(ws), level);
}

/*! Clipping of *vmr_field*.

The method allows you to apply hard limits the values of *vmr_field*.
All values, of the species selected, below *limit_low*, are simply
set to *limit_low*. And the same is performed with respect to
*limit_high*. That is, the data in x for the retrieval quantity are
forced to be inside the range [limit_low,limit_high].

Setting species="ALL", is a shortcut for applying the limits on all
species.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] species - Name of species to consider, or "ALL".
@param[in] limit_low - Lower limit for clipping. (default:
-std::numeric_limits<Numeric>::infinity())
@param[in] limit_high - Upper limit for clipping. (default:
std::numeric_limits<Numeric>::infinity())

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void vmr_fieldClip(
    Workspace& ws, const String& species,
    const Numeric& limit_low = -std::numeric_limits<Numeric>::infinity(),
    const Numeric& limit_high = std::numeric_limits<Numeric>::infinity()) {
  vmr_fieldClip(Var::vmr_field(ws), Var::abs_species(ws), species, limit_low,
                limit_high, Var::verbosity(ws));
}

/*! Adds a perturbation to *vmr_field*.

Works as *AtmFieldPerturb* but acts on *vmr_field*.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] species - Name of species to perturb.
@param[in] p_ret_grid - Pressure retrieval grid.
@param[in] lat_ret_grid - Latitude retrieval grid.
@param[in] lon_ret_grid - Longitude retrieval grid.
@param[in] pert_index - Index of position where the perturbation shall be
performed.
@param[in] pert_size - Size of perturbation.
@param[in] pert_mode - Type of perturbation, ansolute or relative. (default:
"absolute")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void vmr_fieldPerturb(Workspace& ws, const String& species,
                      const Vector& p_ret_grid, const Vector& lat_ret_grid,
                      const Vector& lon_ret_grid, const Index& pert_index,
                      const Numeric& pert_size,
                      const String& pert_mode = "absolute") {
  vmr_fieldPerturb(Var::vmr_field(ws), Var::atmosphere_dim(ws), Var::p_grid(ws),
                   Var::lat_grid(ws), Var::lon_grid(ws), Var::abs_species(ws),
                   species, p_ret_grid, lat_ret_grid, lon_ret_grid, pert_index,
                   pert_size, pert_mode, Var::verbosity(ws));
}

/*! Adds a perturbation to *vmr_field*.

Works as *AtmFieldPerturbAtmGrids* but acts on *vmr_field*.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] species - Name of species to perturb.
@param[in] pert_index - Index of position where the perturbation shall be
performed.
@param[in] pert_size - Size of perturbation.
@param[in] pert_mode - Type of perturbation, ansolute or relative. (default:
"absolute")

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void vmr_fieldPerturbAtmGrids(Workspace& ws, const String& species,
                              const Index& pert_index, const Numeric& pert_size,
                              const String& pert_mode = "absolute") {
  vmr_fieldPerturbAtmGrids(
      Var::vmr_field(ws), Var::atmosphere_dim(ws), Var::p_grid(ws),
      Var::lat_grid(ws), Var::lon_grid(ws), Var::abs_species(ws), species,
      pert_index, pert_size, pert_mode, Var::verbosity(ws));
}

/*! Sets the VMR of all species to a select constant value.

The *vmr_field* WSM must have a correct size before calling this method.
The length of vmr_values and of abs_species must match.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] vmr_values - VMR value to apply for each abs_species.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void vmr_fieldSetAllConstant(Workspace& ws, const Vector& vmr_values) {
  vmr_fieldSetAllConstant(Var::vmr_field(ws), Var::abs_species(ws), vmr_values,
                          Var::verbosity(ws));
}

/*! Sets the VMR of a species to a constant value.

The *vmr_field* WSM must have a correct size before calling this method.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] species - Species to set.
@param[in] vmr_value - VMR value to apply for the selected species.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void vmr_fieldSetConstant(Workspace& ws, const String& species,
                          const Numeric& vmr_value) {
  vmr_fieldSetConstant(Var::vmr_field(ws), Var::abs_species(ws), species,
                       vmr_value, Var::verbosity(ws));
}

/*! Calculates *water_p_eq_field* according to Murphy and Koop, 2005.

The saturation pressure is set to the one with respect to water at
temperatures >= 0C, and to the one with respect to ice for <0C.

The saturation pressure with respect to liquid and ice water is
calculated according to Eq. 10 and 7, respectively, of:
Murphy, D. M., & Koop, T. (2005). Review of the vapour pressures of
ice and supercooled water for atmospheric applications. Quarterly
Journal of the Royal Meteorological Society, 131(608), 1539-1565.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void water_p_eq_fieldMK05(Workspace& ws) {
  water_p_eq_fieldMK05(Var::water_p_eq_field(ws), Var::t_field(ws),
                       Var::verbosity(ws));
}

/*! Maps the planet's rotation to an imaginary wind.

This method is of relevance if the observation platform is not
following the planet's rotation, and Doppler effects must be
considered. Examples include full disk observations from another
planet or a satellite not in orbit of the observed planet.

The rotation of the planet is not causing any Doppler shift for
1D and 2D simulations, and the method can only be used for 3D.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void wind_u_fieldIncludePlanetRotation(Workspace& ws) {
  wind_u_fieldIncludePlanetRotation(
      Var::wind_u_field(ws), Var::atmosphere_dim(ws), Var::p_grid(ws),
      Var::lat_grid(ws), Var::lon_grid(ws), Var::refellipsoid(ws),
      Var::z_field(ws), Var::planet_rotation_period(ws), Var::verbosity(ws));
}

/*! Maps *x* to atmospheric and surface variables.

Maps OEM's state vector, *x*, to the matching ARTS variables. This
method handles atmospheric and surface variables. If you retrieve
other variables, make sure that you also call *x2artsSensor* and/or
*x2artsSpectroscopy*.

The following retrieval quantities are handled by this method:
   Temperature
   Absorption species
   Scattering species
   Winds
   Surface variables

Should only be used inside *inversion_iterate_agenda*.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void x2artsAtmAndSurf(Workspace& ws) {
  x2artsAtmAndSurf(
      ws, Var::vmr_field(ws), Var::t_field(ws),
      Var::particle_bulkprop_field(ws), Var::wind_u_field(ws),
      Var::wind_v_field(ws), Var::wind_w_field(ws), Var::mag_u_field(ws),
      Var::mag_v_field(ws), Var::mag_w_field(ws), Var::surface_props_data(ws),
      Var::jacobian_quantities(ws), Var::x(ws), Var::atmfields_checked(ws),
      Var::atmgeom_checked(ws), Var::atmosphere_dim(ws), Var::p_grid(ws),
      Var::lat_grid(ws), Var::lon_grid(ws), Var::abs_species(ws),
      Var::cloudbox_on(ws), Var::cloudbox_checked(ws),
      Var::particle_bulkprop_names(ws), Var::surface_props_names(ws),
      Var::water_p_eq_agenda(ws), Var::verbosity(ws));
}

/*! Maps *x* to sensor variables.

Maps OEM's state vector, *x*, to the matching ARTS variables. This
method handles variables associated with the sensor. If you retrieve
other variables, make sure that you also call *x2artsAtmAndSurf*
 and/or *x2artsSpectroscopy*.

The following retrieval quantities are handled by this method:
   Pointing
   Frequency shift and stretch
   Baseline fits

Should only be used inside *inversion_iterate_agenda*.

Elements in *x* representing pointing corrections are mapped to
*sensor_los*. Elements representing frequency corrections are mapped
to *f_backend*. Baseline variables are mapped to *y_baseline*.

The sensor response is recalculated if there is any non-zero frequency
correction.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void x2artsSensor(Workspace& ws) {
  x2artsSensor(
      ws, Var::sensor_los(ws), Var::f_backend(ws), Var::y_baseline(ws),
      Var::sensor_response(ws), Var::sensor_response_f(ws),
      Var::sensor_response_pol(ws), Var::sensor_response_dlos(ws),
      Var::sensor_response_f_grid(ws), Var::sensor_response_pol_grid(ws),
      Var::sensor_response_dlos_grid(ws), Var::mblock_dlos_grid(ws),
      Var::jacobian_quantities(ws), Var::x(ws), Var::sensor_response_agenda(ws),
      Var::sensor_checked(ws), Var::sensor_time(ws), Var::verbosity(ws));
}

/*! Just defined to indicate a future extensiom.

Don't call the method, it will just generate an error.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void x2artsSpectroscopy(Workspace& ws) {
  x2artsSpectroscopy(Var::verbosity(ws));
}

/*! Clipping of the state vector.

The method allows you to apply hard limits the values of a
retrieval quantity. The retrieval quantity is specified by
*ijq*. All values of the quantity below *limit_low*, are simply
set to *limit_low*. And the same is performed with respect to
*limit_high*. That is, the data in x for the retrieval quantity
are forced to be inside the range [limit_low,limit_high].

Setting ijq=-1, is a shortcut for applying the limits on all
retrieval quantities.

Notice that limits must be specified in the unit used in *x*.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] ijq - Retrieval quantity index (zero-based)
@param[in] limit_low - Lower limit for clipping. (default:
-std::numeric_limits<Numeric>::infinity())
@param[in] limit_high - Upper limit for clipping. (default:
std::numeric_limits<Numeric>::infinity())

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void xClip(
    Workspace& ws, const Index& ijq,
    const Numeric& limit_low = -std::numeric_limits<Numeric>::infinity(),
    const Numeric& limit_high = std::numeric_limits<Numeric>::infinity()) {
  xClip(Var::x(ws), Var::jacobian_quantities(ws), ijq, limit_low, limit_high,
        Var::verbosity(ws));
}

/*! Standard function for creating *xa*.

The method creates *xa* based on *jacobian_quantities* and the various
atmospheric fields. In the case of scattering species, the data are
taken from *particle_bulkprop_field*. The following retrieval quantities
are handled:
   Temperature
   Absorption species
   Scattering species
   Pointing
   Polynomial baseline fit
   Sinusoidal baseline fit

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void xaStandard(Workspace& ws) {
  xaStandard(ws, Var::xa(ws), Var::jacobian_quantities(ws),
             Var::atmfields_checked(ws), Var::atmgeom_checked(ws),
             Var::atmosphere_dim(ws), Var::p_grid(ws), Var::lat_grid(ws),
             Var::lon_grid(ws), Var::t_field(ws), Var::vmr_field(ws),
             Var::abs_species(ws), Var::cloudbox_on(ws),
             Var::cloudbox_checked(ws), Var::particle_bulkprop_field(ws),
             Var::particle_bulkprop_names(ws), Var::wind_u_field(ws),
             Var::wind_v_field(ws), Var::wind_w_field(ws), Var::mag_u_field(ws),
             Var::mag_v_field(ws), Var::mag_w_field(ws),
             Var::surface_props_data(ws), Var::surface_props_names(ws),
             Var::water_p_eq_agenda(ws), Var::verbosity(ws));
}

/*! Replaces *yCalc* for radar/lidar calculations.

The output format for *iy* when simulating radars and lidars differs
from the standard one, and *yCalc* can not be used for such simulations.
This method works largely as *yCalc*, but is tailored to handle the
output from *iyActiveSingleScat*.

The method requires additional information about the sensor,
regarding its recieving properties. First of all, recieved
polarisation states are taken from *instrument_pol_array*. Note
that this WSV allows to define several measured polarisations
for each transmitted signal. For example, it is possible to
simulate transmission of V and measuring backsacttered V and H.

Secondly, the range averaging is described by *range_bins*. These
bins can either be specified in altitude or two-way travel time.
In both case, the edges of the range bins shall be specified.
All data (including auxiliary variables) are returned as the
average inside the bins. If a bin is totally outside the model
atmosphere, NaN is returned.

The options for *iy_unit* are:
 "1"   : Backscatter coefficient. Unit is 1/(m*sr). At zero
           attenuation, this equals the scattering matrix value for
           the backward direction. See further AUG.
 "Ze"  : Equivalent reflectivity. Unit is mm^6/m^3. Conversion
           formula is given below.
 "dBZe": 10*log10(Ze/Z0), where Z0 is 1 mm^6/m^3.

The conversion from backscatter coefficient to Ze is:
   Ze = 1e18 * lambda^4 / (k2 * pi^5) * sum(sigma),
where sum(sigma) = 4 * pi * b, and b is the backscatter coefficient.

The reference dielectric factor can either specified directly by
the argument *k2*. For example, to mimic the CloudSat data, *k2*
shall be set to 0.75 (citaion needed). If *k2* is set to be
negative (which is defualt), k2 is calculated as:
   k2 = abs( (n^2-1)/(n^2+2) )^2,
where n is the refractive index of liquid water at temperature
*ze_tref* and the frequency of the radar, calculated by the MPM93
parameterization.

A lower limit for dBZe is applied (*dbze_min*). The main reason is to
handle the fact dBZe is not defined for Ze=0, and dBZe is set to the
clip value when Ze < 10^(dbze_min/10).

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] ze_tref - Reference temperature for conversion to Ze. (default:
273.15)
@param[in] k2 - Reference dielectric factor. (default: -1)
@param[in] dbze_min - Clip value for dBZe. (default: -99)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void yActive(Workspace& ws, const Numeric& ze_tref = 273.15,
             const Numeric& k2 = -1, const Numeric& dbze_min = -99) {
  yActive(ws, Var::y(ws), Var::y_f(ws), Var::y_pol(ws), Var::y_pos(ws),
          Var::y_los(ws), Var::y_aux(ws), Var::y_geo(ws), Var::jacobian(ws),
          Var::atmgeom_checked(ws), Var::atmfields_checked(ws),
          Var::iy_unit(ws), Var::iy_aux_vars(ws), Var::stokes_dim(ws),
          Var::f_grid(ws), Var::atmosphere_dim(ws), Var::nlte_field(ws),
          Var::cloudbox_on(ws), Var::cloudbox_checked(ws), Var::sensor_pos(ws),
          Var::sensor_los(ws), Var::sensor_checked(ws), Var::jacobian_do(ws),
          Var::jacobian_quantities(ws), Var::iy_main_agenda(ws),
          Var::geo_pos_agenda(ws), Var::instrument_pol_array(ws),
          Var::range_bins(ws), ze_tref, k2, dbze_min, Var::verbosity(ws));
}

/*! Extraction of arbitrary linear polarisation.

This method shall be called after *yCalc* and then applies *sensor_pol*
on the outout of *yCalc*. See *sensor_pol* for definition of the
polarisation responses. THe *sensor_response* give to *yCalc* can not
contain any polarisation response, it must maintain original Stokes
elelemnts. The value of *stokes_dim* muist be >= 3.

The values in *sensor_pol* are applied on *y*, and *jacobian* if relevant.
*y_pol* is set following the values in *sensor_pol* but is rounded to
an integer value. Remaining data associated with *y* (e.g. y_pos) are
set to the value matching the first Stokes element.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void yApplySensorPol(Workspace& ws) {
  yApplySensorPol(Var::y(ws), Var::y_f(ws), Var::y_pol(ws), Var::y_pos(ws),
                  Var::y_los(ws), Var::y_aux(ws), Var::y_geo(ws),
                  Var::jacobian(ws), Var::stokes_dim(ws), Var::jacobian_do(ws),
                  Var::sensor_pos(ws), Var::sensor_pol(ws), Var::verbosity(ws));
}

/*! Conversion of *y* to other spectral units.

Any conversion to brightness temperature is normally made inside
*yCalc*. This method makes it possible to also make this conversion
after *yCalc*, but with restrictions for *jacobian* and with.
respect to the n2-law of radiance.

The conversion made inside *iyEmissionStandard* is mimiced
and see that method for constraints and selection of output units.
This with the restriction that the n2-law can be ignored. The later
is the case if the sensor is placed in space, or if the refractive
only devaites slightly from unity.

The method handles *y* and *jacobian* in parallel, where
the last variable is only considered if it is set. The
input data must be in original radiance units. A completely
stringent check of this can not be performed.

The method can not be used with jacobian quantities that are not
obtained through radiative transfer calculations. One example on
quantity that can not be handled is *jacobianAddPolyfit*. There
are no automatic checks warning for incorrect usage!

If you are using this method, *iy_unit* should be set to "1" when
calling *yCalc*, and be changed before calling this method.

Conversion of *y_aux* is not supported.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void yApplyUnit(Workspace& ws) {
  yApplyUnit(Var::y(ws), Var::jacobian(ws), Var::y_f(ws), Var::y_pol(ws),
             Var::iy_unit(ws), Var::verbosity(ws));
}

/*! Calculation of complete measurement vectors (y).

The method performs radiative transfer calculations from a sensor
perspective. Radiative transfer calculations are performed for
monochromatic pencil beams, following *iy_main_agenda* and
associated agendas. Obtained radiances are weighted together by
*sensor_response*, to include the characteristics of the sensor.
The measurement vector obtained can contain anything from a single
frequency value to a series of measurement scans (each consisting
of a series of spectra), all depending on the settings. Spectra
and jacobians are calculated in parallel.

The frequency, polarisation etc. for each measurement value is
given by *y_f*, *y_pol*, *y_pos* and *y_los*.

The content of *y_aux* follows *iy_aux_vars. See the method selected
for *iy_main_agenda* for allowed choices.

The geo-positions (*y_geo*) are set based on *sensor_response*. When
an antenna pattern is considered, there are several pencil beams,
and thus also several goe-positions, associated with each value of *y*.
The geo-position assigned to a value in *y* is the *geo_pos* of the pencil
beam related to the highest value in *sensor_response*. This means that
*mblock_dlos_grid* must contain the bore-sight direction (0,0), if you
want *y_geo* to exactly match the bore-sight direction.

The Jacobian provided (*jacobian*) is adopted to selected retrieval
units, but no transformations are applied. Transformations are
included by calling *jacobianAdjustAndTransform*.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void yCalc(Workspace& ws) {
  yCalc(ws, Var::y(ws), Var::y_f(ws), Var::y_pol(ws), Var::y_pos(ws),
        Var::y_los(ws), Var::y_aux(ws), Var::y_geo(ws), Var::jacobian(ws),
        Var::atmgeom_checked(ws), Var::atmfields_checked(ws),
        Var::atmosphere_dim(ws), Var::nlte_field(ws), Var::cloudbox_on(ws),
        Var::cloudbox_checked(ws), Var::scat_data_checked(ws),
        Var::sensor_checked(ws), Var::stokes_dim(ws), Var::f_grid(ws),
        Var::sensor_pos(ws), Var::sensor_los(ws), Var::transmitter_pos(ws),
        Var::mblock_dlos_grid(ws), Var::sensor_response(ws),
        Var::sensor_response_f(ws), Var::sensor_response_pol(ws),
        Var::sensor_response_dlos(ws), Var::iy_unit(ws),
        Var::iy_main_agenda(ws), Var::geo_pos_agenda(ws),
        Var::jacobian_agenda(ws), Var::jacobian_do(ws),
        Var::jacobian_quantities(ws), Var::iy_aux_vars(ws), Var::verbosity(ws));
}

/*! Replaces *yCalc* if a measurement shall be appended to an
existing one.

The method works basically as *yCalc* but appends the results to
existing data, instead of creating completely new *y* and its
associated variables. This method is required if your measurement
consists of data from two instruments using different observation
techniques (corresponding to different iyCalc-methods). One such
example is if emission and transmission data are combined into a
joint retrieval. The method can also be used to get around the
constrain that *sensor_response* is required to be the same for
all data.

The new measurement is simply appended to the input *y*, and the
other output variables are treated correspondingly. Data are
appended "blindly" in *y_aux*. That is, data of different type
are appended if *iy_aux_vars* differs between the two measurements,
the data are appended strictly following the order. First variable
of second measurement is appended to first variable of first
measurement, and so on. The number of auxiliary variables can differ
between the measurements. Missing data are set to zero.

The set of retrieval quantities can differ between the two
calculations. If an atmospheric quantity is part of both Jacobians,
the same retrieval grids must be used in both cases.
The treatment of instrument related Jacobians (baseline fits,
pointing ...) follows the *append_instrument_wfs* argument.

A difference to *yCalc* is that *jacobian_quantities* is both in-
and output variable. The input version shall match the measurement
to be calculated, while the output version matches the output *y*,
the combined, measurements. A copies of *jacobian_quantities* of the
first measurement must be made and shall be provided to the method
as *jacobian_quantities_copy*.

As for *yCalc* Jacobian transformations are not handled, and the
the input Jacobian shall not contain transformations. That is
*jacobianAdjustAndTransform* shall be called after this method,
when the complete Jacobian is at hand.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] jacobian_quantities_copy - Copy of *jacobian_quantities* of first
measurement.
@param[in] append_instrument_wfs - Flag controlling if instrumental weighting
functions are appended or treated as different retrieval quantities. (default:
0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void yCalcAppend(Workspace& ws,
                 const ArrayOfRetrievalQuantity& jacobian_quantities_copy,
                 const Index& append_instrument_wfs = 0) {
  yCalcAppend(
      ws, Var::y(ws), Var::y_f(ws), Var::y_pol(ws), Var::y_pos(ws),
      Var::y_los(ws), Var::y_aux(ws), Var::y_geo(ws), Var::jacobian(ws),
      Var::jacobian_quantities(ws), Var::atmgeom_checked(ws),
      Var::atmfields_checked(ws), Var::atmosphere_dim(ws), Var::nlte_field(ws),
      Var::cloudbox_on(ws), Var::cloudbox_checked(ws),
      Var::scat_data_checked(ws), Var::sensor_checked(ws), Var::stokes_dim(ws),
      Var::f_grid(ws), Var::sensor_pos(ws), Var::sensor_los(ws),
      Var::transmitter_pos(ws), Var::mblock_dlos_grid(ws),
      Var::sensor_response(ws), Var::sensor_response_f(ws),
      Var::sensor_response_pol(ws), Var::sensor_response_dlos(ws),
      Var::iy_unit(ws), Var::iy_main_agenda(ws), Var::geo_pos_agenda(ws),
      Var::jacobian_agenda(ws), Var::jacobian_do(ws), Var::iy_aux_vars(ws),
      jacobian_quantities_copy, append_instrument_wfs, Var::verbosity(ws));
}

/*! Computes *y* from input using standard calibration scheme of cold-atm-hot
observations

If calib evaluates as true:
    y = cold_temp + (hot_temp - cold_temp) * (atm - cold) / (hot - cold)

If calib evaluates as false:
    y = (hot_temp * cold - cold_temp * hot) / (hot - cold)

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] cold - N-elem Vector of cold load linear power
@param[in] atm - N-elem Vector of atmosphere linear power
@param[in] hot - N-elem Vector of hot load linear power
@param[in] cold_temp - Cold load temperature
@param[in] hot_temp - Hot load temperature
@param[in] calib - Flag for calibration scheme, false means system temperature
is computed (default: 1)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void yColdAtmHot(Workspace& ws, const Vector& cold, const Vector& atm,
                 const Vector& hot, const Numeric& cold_temp,
                 const Numeric& hot_temp, const Index& calib = 1) {
  yColdAtmHot(Var::y(ws), cold, atm, hot, cold_temp, hot_temp, calib,
              Var::verbosity(ws));
}

/*! Converts *iy* to *y* assuming a fixed frequency resolution.

This is a short-cut, avoiding *yCalc*, that can be used to convert
monochromatic pencil beam data to spectra with a fixed resolution.

The method mimics a spectrometer with rectangular response
functions, all having the same width (*df*). The position of
the first spectrometer channel is set to f_grid[0]+df/2.
The centre frequency of channels are returned as *y_f*.

Auxiliary variables and *jacobian*s are not handled.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] df - Selected frequency resolution.

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ySimpleSpectrometer(Workspace& ws, const Numeric& df) {
  ySimpleSpectrometer(Var::y(ws), Var::y_f(ws), Var::iy(ws),
                      Var::stokes_dim(ws), Var::f_grid(ws), df,
                      Var::verbosity(ws));
}

/*! Performs batch calculations for the measurement vector y.

We perform *ybatch_n* jobs, starting at index *ybatch_start*. (Zero
based indexing, as usual.) The output array *ybatch* will have
ybatch_n elements. Indices in the output array start
with zero, independent of *ybatch_start*.

The method performs the following:
   1. Sets *ybatch_index* = *ybatch_start*.
   2. Performs a-d until
      *ybatch_index* = *ybatch_start* + *ybatch_n*.
        a. Executes *ybatch_calc_agenda*.
        b. If *ybatch_index* = *ybatch_start*, resizes *ybatch*
           based on *ybatch_n* and length of *y*.
        c. Copies *y* to *ybatch_index* - *ybatch_start*
           of *ybatch*.
        d. Adds 1 to *ybatch_index*.

Beside the *ybatch_calc_agenda*, the WSVs *ybatch_start*
and *ybatch_n* must be set before calling this method.
Further, *ybatch_calc_agenda* is expected to produce a
spectrum and should accordingly include a call of *yCalc*
(or asimilar method).

The input variable *ybatch_start* is set to a default of zero in
*general.arts*.

An agenda that calculates spectra for different temperature profiles
could look like this:

   AgendaSet(ybatch_calc_agenda){
      Extract(t_field,tensor4_1,ybatch_index)
      yCalc
   }

Jacobians are also collected, and stored in output variable *ybatch_jacobians*.
(This will be empty if yCalc produces empty Jacobians.)

See the user guide for further practical examples.

@author Stefan Buehler

@param[in,out] Workspace ws - An ARTS workspace
@param[in] robust - A flag with value 1 or 0. If set to one, the batch
calculation will continue, even if individual jobs fail. In
that case, a warning message is written to screen and file
(out1 output stream), and the *y* Vector entry for the
failed job in *ybatch* is left empty. (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ybatchCalc(Workspace& ws, const Index& robust = 0) {
  ybatchCalc(ws, Var::ybatch(ws), Var::ybatch_aux(ws),
             Var::ybatch_jacobians(ws), Var::ybatch_start(ws),
             Var::ybatch_n(ws), Var::ybatch_calc_agenda(ws), robust,
             Var::verbosity(ws));
}

/*! This method is used for simulating ARTS for metoffice model fields
This method reads in *met_amsu_data* which contains the
lat-lon of the metoffice profile files as a Matrix. It then
loops over the number of profiles and corresponding to each
longitude create the appropriate profile basename. Then,
corresponding to each basename we have temperature field, altitude
field, humidity field, and particle number density field. The
temperature field and altitude field are stored in the same dimensions
as *t_field_raw* and *z_field_raw*. The oxygen and nitrogen VMRs are
set to constant values of 0.209 and 0.782, respectively and are used
along with humidity field to generate *vmr_field_raw*.

The three fields *t_field_raw*, *z_field_raw*, and *vmr_field_raw* are
given as input to *met_profile_calc_agenda* which is called in this
method. See documentation of WSM *met_profile_calc_agenda* for more
information on this agenda.

The method also converts satellite zenith angle to appropriate
*sensor_los*. It also sets the *p_grid* and *cloudbox_limits*
from the profiles inside the function

@author Sreerekha T.R.

@param[in,out] Workspace ws - An ARTS workspace
@param[in] nelem_p_grid - FIXME DOC
@param[in] met_profile_path - FIXME DOC
@param[in] met_profile_pnd_path - FIXME DOC

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ybatchMetProfiles(Workspace& ws, const Index& nelem_p_grid,
                       const String& met_profile_path,
                       const String& met_profile_pnd_path) {
  ybatchMetProfiles(ws, Var::ybatch(ws), Var::abs_species(ws),
                    Var::met_profile_calc_agenda(ws), Var::f_grid(ws),
                    Var::met_amsu_data(ws), Var::sensor_pos(ws),
                    Var::refellipsoid(ws), Var::lat_grid(ws), Var::lon_grid(ws),
                    Var::atmosphere_dim(ws), Var::scat_data(ws), nelem_p_grid,
                    met_profile_path, met_profile_pnd_path, Var::verbosity(ws));
}

/*! This method is used for simulating ARTS for metoffice model fields
for clear sky conditions.

This method reads in *met_amsu_data* which contains the
lat-lon of the metoffice profile files as a Matrix. It then
loops over the number of profiles and corresponding to each
longitude create the appropriate profile basename. Then,
Corresponding to each basename we have temperature field, altitude
field, humidity field, and particle number density field. The
temperature field and altitude field are stored in the same dimensions
as *t_field_raw* and *z_field_raw*. The oxygen and nitrogen VMRs are
set to constant values of 0.209 and 0.782, respectively and are used
along with humidity field to generate *vmr_field_raw*.

The three fields *t_field_raw*, *z_field_raw*, and *vmr_field_raw* are
given as input to *met_profile_calc_agenda* which is called in this
method. See documentation of WSM *met_profile_calc_agenda* for more
information on this agenda.

The method also converts satellite zenith angle to appropriate
*sensor_los*. It also sets the *p_grid* and *cloudbox_limits*
from the profiles inside the function

@author Seerekha T.R.

@param[in,out] Workspace ws - An ARTS workspace
@param[in] nelem_p_grid - FIXME DOC
@param[in] met_profile_path - FIXME DOC

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ybatchMetProfilesClear(Workspace& ws, const Index& nelem_p_grid,
                            const String& met_profile_path) {
  ybatchMetProfilesClear(ws, Var::ybatch(ws), Var::abs_species(ws),
                         Var::met_profile_calc_agenda(ws), Var::f_grid(ws),
                         Var::met_amsu_data(ws), Var::sensor_pos(ws),
                         Var::refellipsoid(ws), nelem_p_grid, met_profile_path,
                         Var::verbosity(ws));
}

/*! Time average of *ybatch* and *time_grid*

Computes the internal covariance matrix in *covmat_sepsbatch*, and
stores the number of elements per averaging in *counts*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] time_step - Time step in the form "INDEX SCALE", where SCALE is "h",
"min", or "s" for hours, minutes or seconds
@param[in] disregard_first - Flag to remove first time step (e.g., if it is an
incomplete step) (default: 0)
@param[in] disregard_last - Flag to remove last time step (e.g., if it is an
incomplete step) (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ybatchTimeAveraging(Workspace& ws, const String& time_step,
                         const Index& disregard_first = 0,
                         const Index& disregard_last = 0) {
  ybatchTimeAveraging(Var::ybatch(ws), Var::time_grid(ws),
                      Var::covmat_sepsbatch(ws), Var::counts(ws), time_step,
                      disregard_first, disregard_last, Var::verbosity(ws));
}

/*! Performs naive tropospheric corrections on *ybatch*

Sets *ybatch_corr* to be able to perform the inverse of the corrections,
each array-element with 3 entries as [median, part_trans, trop_temp]

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] range - Positions where the median of the baseline is computed, if
empty all is used
@param[in] trop_temp - Radiative temperature of the troposphere
@param[in] targ_temp - Temperature target of the baseline (default: 2.73)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ybatchTroposphericCorrectionNaiveMedianForward(
    Workspace& ws, const ArrayOfIndex& range, const Vector& trop_temp,
    const Numeric& targ_temp = 2.73) {
  ybatchTroposphericCorrectionNaiveMedianForward(
      Var::ybatch_corr(ws), Var::ybatch(ws), range, trop_temp, targ_temp,
      Var::verbosity(ws));
}

/*! Performs inverse of naive tropospheric corrections on *ybatch*

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void ybatchTroposphericCorrectionNaiveMedianInverse(Workspace& ws) {
  ybatchTroposphericCorrectionNaiveMedianInverse(
      Var::ybatch(ws), Var::ybatch_corr(ws), Var::verbosity(ws));
}

/*! Force altitudes to fulfil hydrostatic equilibrium.

The method applies hydrostatic equilibrium. A mixture of "dry
air" and water vapour (if present as *abs_species* tag) is assumed.
That is, the air is assumed to be well mixed and its weight, apart
from the water vapour, is constant (*molarmass_dry_air*). In
addition, the effect of any particles (including liquid and ice
particles) is neglected.

The output is an update of *z_field*. This variable is expected to
contain approximative altitudes when calling the function. The
altitude matching *p_hse* is kept constant. Other input altitudes can
basically be arbitrary, but good estimates give quicker calculations.

The calculations are repeated until the change in altitude is below
*z_hse_accuracy*. An iterative process is needed as gravity varies
with altitude.

For 1D and 2D, the geographical position is taken from *lat_true*
and *lon_true*.

@author Patrick Eriksson

@param[in,out] Workspace ws - An ARTS workspace

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void z_fieldFromHSE(Workspace& ws) {
  z_fieldFromHSE(ws, Var::z_field(ws), Var::atmosphere_dim(ws), Var::p_grid(ws),
                 Var::lat_grid(ws), Var::lon_grid(ws), Var::lat_true(ws),
                 Var::lon_true(ws), Var::abs_species(ws), Var::t_field(ws),
                 Var::vmr_field(ws), Var::refellipsoid(ws), Var::z_surface(ws),
                 Var::atmfields_checked(ws), Var::g0_agenda(ws),
                 Var::molarmass_dry_air(ws), Var::p_hse(ws),
                 Var::z_hse_accuracy(ws), Var::verbosity(ws));
}

/*! Sets the surface altitude to a constant. Defaults to zero.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] altitude - The constant altitude. (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void z_surfaceConstantAltitude(Workspace& ws, const Numeric& altitude = 0) {
  z_surfaceConstantAltitude(Var::z_surface(ws), Var::lat_grid(ws),
                            Var::lon_grid(ws), altitude, Var::verbosity(ws));
}

/*! Sets the surface altitude for a given latitude and longitude grid.

@author Richard Larsson

@param[in,out] Workspace ws - An ARTS workspace
@param[in] filename - File of GriddedField2 with surface altitudes gridded
@param[in] interp_order - Interpolation order (default: 1)
@param[in] set_lowest_altitude_to_zero - Index that sets the lowest altitude to
0 to ignore sub-surface pressures/altitudes (default: 0)

Use the ARTS documentation to read more on how the workspace is manipulated
*/
void z_surfaceFromFileAndGrid(Workspace& ws, const String& filename,
                              const Index& interp_order = 1,
                              const Index& set_lowest_altitude_to_zero = 0) {
  z_surfaceFromFileAndGrid(Var::z_surface(ws), Var::lat_grid(ws),
                           Var::lon_grid(ws), filename, interp_order,
                           set_lowest_altitude_to_zero, Var::verbosity(ws));
}

}  // namespace ARTS::Method
